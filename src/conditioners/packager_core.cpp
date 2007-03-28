/*
 * Copyright 2006 Sony Computer Entertainment Inc.
 *
 * Licensed under the SCEA Shared Source License, Version 1.0 (the "License"); you may not use this 
 * file except in compliance with the License. You may obtain a copy of the License at:
 * http://research.scea.com/scea_shared_source_license.html
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License 
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
 * implied. See the License for the specific language governing permissions and limitations under the 
 * License. 
 */

#define WINDLL
#include "packager.h"
#include "zip.h"
#include <iostream>
#include <string>
#include <fstream>
#include <direct.h>

#define ZIP_DLL_NAME "ZIP32.DLL\0"
#define TEMPDIR		"./~packagertemp/"

int AddPathName(string str);
string FindFileName(string str);
string FindPath(string str);
string FindFile(string str);
string FindFileInSubDirs(string path, string file);

string daePath;
vector<string> filepath_vector;

typedef int (WINAPI * _DLL_ZIP)(ZCL);
typedef int (WINAPI * _ZIP_USER_FUNCTIONS)(LPZIPUSERFUNCTIONS);
typedef BOOL (WINAPI * ZIPSETOPTIONS)(LPZPOPT);

/* Password entry routine - see password.c in the wiz directory for how
   this is actually implemented in Wiz. If you have an encrypted file,
   this will probably give you great pain. Note that none of the
   parameters are being used here, and this will give you warnings.
 */
int WINAPI DummyPassword(LPSTR p, int n, LPCSTR m, LPCSTR name)
{
return 1;
}

/* Dummy "print" routine that simply outputs what is sent from the dll */
int WINAPI DummyPrint(char far *buf, unsigned long size)
{
//PRINTF("%s", buf);
return (unsigned int) size;
}

/* Dummy "comment" routine. See comment.c in the wiz directory for how
   this is actually implemented in Wiz. This will probably cause you
   great pain if you ever actually make a call into it.
 */
int WINAPI DummyComment(char far *szBuf)
{
szBuf[0] = '\0';
return TRUE;
}

int AddPathName(string str)
{
	for (unsigned int i=0; i < (unsigned int) filepath_vector.size(); i++) {
		if (filepath_vector[i] == str)
			return 0;          // file already added
	}
	filepath_vector.insert(filepath_vector.begin(), str);
	return 0;
}

BOOL FileExist(string str)
{
	if (str == "") return false;
	fstream file(str.c_str(),ios::in);
	if( file.is_open() ) {
		file.close();
		return true;
	}
	return false;
}

string FindFile(string str)
{
	xsAnyURI uri(str.c_str());
	string temp, filename, pathname;
	const char * protocol = uri.getProtocol();
	if (protocol) {
		if (stricmp(protocol, "file") == 0)
		{
			string slashpathname = uri.getFilepath();
			pathname = slashpathname.substr(1, slashpathname.length());
		} else 	if (stricmp(protocol, "http") == 0)
		{
			return "";
		}
	}
	if (FileExist(str.c_str()))
		return str;
	if (FileExist(pathname.c_str()))
		return pathname;
	filename = FindFileName(str);
	if (FileExist(pathname.c_str()))
		return pathname;
	temp = daePath + "/" + str;
	if (FileExist(temp.c_str()))
		return temp;
	temp = daePath + "/" + filename;
	if (FileExist(temp.c_str()))
		return temp;
	return FindFileInSubDirs(daePath, FindFileName(str));
}

string FindFileName(string str)
{
	int backslash = (int) str.find_last_of("/");
	int forwardslash = (int) str.find_last_of("\\");
	if ((backslash < 0) && (forwardslash < 0))
		return str;
	int lastslash = backslash < forwardslash ? forwardslash : backslash;
	return str.substr(lastslash+1,str.size()-(lastslash+1));
}

string FindPath(string str)
{
	int backslash = (int) str.find_last_of("/");
	int forwardslash = (int) str.find_last_of("\\");
	if ((backslash < 0) && (forwardslash < 0))
		return "";
	int lastslash = backslash < forwardslash ? forwardslash : backslash;
	int filestrlen = (int) strlen("file:///");
	string file = str.substr(0, filestrlen);
	if (stricmp(file.c_str(), "file:///") == 0)
		return str.substr(filestrlen,lastslash-filestrlen);
	return str.substr(0,lastslash);
}

string FindFileInSubDirs(string path, string file)
{
	WIN32_FIND_DATA FindFileData;
	string searchstr = path + "/*";
	HANDLE hfile = FindFirstFile(searchstr.c_str(), &FindFileData);
	do {
		string filename = FindFileData.cFileName;
		if ((filename == ".") || (filename == "..")) {
			continue;
		}
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			string nextpath = path + "/" + filename;
			string newpath = FindFileInSubDirs(nextpath, file);
			if (newpath != "") return newpath; 
		}
		if (FindFileData.cFileName == file) {
			string temp = path + "/" + filename;
			fstream filestream(temp.c_str(),ios::in);
			if( filestream.is_open() ) {
				filestream.close();
				return temp;
			}
		}
	} while (FindNextFile(hfile, &FindFileData));
	return "";
}

vector<string> Packager::gatherExternals( const daeString &docName, bool needsTemp, bool verbose )
{
	vector<string> tempfiles;
	int error = 0;
	
	daeDatabase* db = _dae->getDatabase();
	//TODO: Clone all secondary documents before modifying them.
	daeDocument* doc = db->getDocument( docName );
	if (needsTemp)
	{
		daeDocument *tmp;
		string tempName = string("packager-temp_") + string(doc->getDocumentURI()->getFile());
		db->insertDocument( tempName.c_str(), doc->getDomRoot()->clone(), &tmp );
		doc = tmp;
	}

	//gather all images needed
	// <image>
	/*unsigned int imageTagCount = _dae->getDatabase()->getElementCount(NULL, "image", docName );
	domImage *thisImage;  // Pointer to the image element we are currently working on
	for(unsigned int i=0; i<imageTagCount; i++)
	{
		// Get the next image element
		error = _dae->getDatabase()->getElement((daeElement**)&thisImage,i, NULL, "image", docName);
		if(error != DAE_OK)
			continue;

		daeURI * uri = thisImage->getDocument()->getDocumentURI();
		daePath = FindPath(uri->getURI());
		if (daePath == "") daePath = ".";
		xsAnyURI imagefileuri = thisImage->getInit_from()->getValue();
		string newuri = FindFile(imagefileuri.getURI());
		if (newuri=="") {
			if (verbose) printf("Reference image %s not found\n", imagefileuri.getURI());		
			continue;
		}
		AddPathName(newuri);
		thisImage->getInit_from()->getValue().setURI(imagefileuri.getFile());		
	}*/

	//look through all external references
	daeStringRefArray ref = doc->getReferencedDocuments();
	for (daeUInt j=0; j<ref.getCount(); j++)
	{
		const daeTArray<daeURI*> * uri_array = doc->getExternalURIs(ref[j]);
		if (uri_array == NULL) continue;
		for (daeUInt k=0; k<(*uri_array).getCount(); k++)
		{
			daeURI* uri = (*uri_array)[k];
			if (uri->getProtocol() && stricmp(uri->getProtocol(), "file") != 0)
				continue;
			if (verbose) printf("reference file = %s\n", uri->getURI());
			string fragment;
			string file;
			if (uri->getID())
				fragment = uri->getID();
			if (uri->getFile())
				file = uri->getFile();
			if (uri->getExtension())
			{
				if (stricmp(uri->getExtension(), "dae") == 0)
				{
					tempfiles = gatherExternals( uri->getURI(), true, verbose );
					string fileandfragment = file + "#" + fragment;
					uri->setURI(fileandfragment.c_str());
				}
				else
				{
					//if not a COLLADA document ie cgfx
					uri->setURI(file.c_str());
					daePath = FindPath(daeString(ref[j]));
					if (daePath == "") daePath = ".";
					string newuri = FindFile(daeString(ref[j]));
					if (newuri=="") {
						if (verbose) printf("Reference document %s not found\n", ref[j]);
						continue;
					}
					AddPathName(newuri);
				}
			}
		}
	}

	// Add modified dae
	daeString xmldoc = doc->getDocumentURI()->getFile();
	string newuri = FindFile(xmldoc);
	string tempfile = string(TEMPDIR) + xmldoc;
	_dae->saveAs(tempfile.c_str(), doc->getDocumentURI()->getURI() );
	AddPathName(tempfile.c_str());
	tempfiles.push_back(tempfile);

	return tempfiles;
}

bool Packager::init()
{
	setNumOutputs(0);
	addBoolOption( "verbose", "verbose", "verbose", true);
	addStringOption( "zip", "archivename", "archivename", "", true);
	return true;
}

int Packager::execute()
{

	string temp = getInput(0);
	string archivename;
	bool verbose(false);
	getBoolOption("verbose", verbose);
	getStringOption("zip", archivename);

	if (archivename == "") //use default name 
	{
		string input = getInput(0);
		string temp = input.substr(1,input.length()-5);
		archivename = temp + ".zip";
	}

	char szFullPath[PATH_MAX];
	int res;
	_DLL_ZIP ZipArchive;
	_ZIP_USER_FUNCTIONS ZipInit;
	ZIPSETOPTIONS ZipSetOptions;
	ZPOPT ZpOpt;
	ZCL ZpZCL;

	char *ptr;
	res = SearchPath(NULL, ZIP_DLL_NAME, NULL, PATH_MAX, szFullPath, &ptr);
	if (res == 0)
	{
		printDebugMessage("Can't find zip32.dll\n");
		return -1;
	}

	HINSTANCE hZipDll = LoadLibrary(ZIP_DLL_NAME);
	if (hZipDll == NULL)
	{
		printDebugMessage("Can't load zip32.dll\n");
		return -1;
	}

	res =mkdir(TEMPDIR);

	vector<string> tempfiles = gatherExternals( getInput(0).c_str(), false, verbose );

	// Infozip
    (_DLL_ZIP)ZipArchive = (_DLL_ZIP)GetProcAddress(hZipDll, "ZpArchive");
	if (!ZipArchive)
	{
		printDebugMessage("Can't get address ZpArchive\n");
		return -1;
	}
    (ZIPSETOPTIONS)ZipSetOptions = (ZIPSETOPTIONS)GetProcAddress(hZipDll, "ZpSetOptions");
	if (!ZipSetOptions)
	{
		printDebugMessage("Can't get address ZpSetOptions\n");
		return -1;
	}
	(_ZIP_USER_FUNCTIONS)ZipInit = (_ZIP_USER_FUNCTIONS)GetProcAddress(hZipDll, "ZpInit");
	if (!ZipInit)
	{
		printDebugMessage("Can't get address ZpInit\n");
		return -1;
	}

	ZIPUSERFUNCTIONS lpZipUserFunctions;
	lpZipUserFunctions.print = DummyPrint;
	lpZipUserFunctions.password = DummyPassword;
	lpZipUserFunctions.comment = DummyComment;
	lpZipUserFunctions.ServiceApplication = NULL;

	/* Here is where the action starts */
	ZpOpt.fVerbose = verbose == 0 ? FALSE : TRUE;
	ZpOpt.fLevel = '9';
	ZpOpt.fSuffix = FALSE;        /* include suffixes (not yet implemented) */
	ZpOpt.fEncrypt = FALSE;       /* true if encryption wanted */
	ZpOpt.fSystem = FALSE;        /* true to include system/hidden files */
	ZpOpt.fVolume = FALSE;        /* true if storing volume label */
	ZpOpt.fExtra = FALSE;         /* true if including extra attributes */
	ZpOpt.fNoDirEntries = TRUE;  /* true if ignoring directory entries */
	ZpOpt.fVerbose = FALSE;       /* true if full messages wanted */
	ZpOpt.fQuiet = FALSE;         /* true if minimum messages wanted */
	ZpOpt.fCRLF_LF = FALSE;       /* true if translate CR/LF to LF */
	ZpOpt.fLF_CRLF = FALSE;       /* true if translate LF to CR/LF */
	ZpOpt.fJunkDir = TRUE;       /* true if junking directory names */
	ZpOpt.fGrow = FALSE;          /* true if allow appending to zip file */
	ZpOpt.fForce = FALSE;         /* true if making entries using DOS names */
	ZpOpt.fMove = FALSE;          /* true if deleting files added or updated */
	ZpOpt.fUpdate = TRUE;        /* true if updating zip file--overwrite only if newer */
	ZpOpt.fFreshen = FALSE;       /* true if freshening zip file--overwrite only */
	ZpOpt.fJunkSFX = FALSE;       /* true if junking sfx prefix*/
	ZpOpt.fLatestTime = FALSE;    /* true if setting zip file time to time of latest file in archive */
	ZpOpt.fComment = FALSE;       /* true if putting comment in zip file */
	ZpOpt.fOffsets = FALSE;       /* true if updating archive offsets for sfx files */
	ZpOpt.fDeleteEntries = FALSE; /* true if deleting files from archive */
	ZpOpt.fRecurse = 0;           /* subdir recursing mode: 1 = "-r", 2 = "-R" */
	ZpOpt.fRepair = 0;            /* archive repair mode: 1 = "-F", 2 = "-FF" */
	ZpOpt.Date = NULL;            /* Not using, set to NULL pointer */
	getcwd(szFullPath, PATH_MAX); /* Set directory to current directory */
	ZpOpt.szRootDir = szFullPath;
	ZpOpt.szTempDir = TEMPDIR;

	ZpZCL.argc = (int) filepath_vector.size();        /* number of files to archive - adjust for the actual number of file names to be added */
	ZpZCL.lpszZipFN = (LPSTR) archivename.c_str();    /* archive to be created/updated */

	char ** filelist;
	filelist = (char **) malloc ((sizeof (char *)) * ZpZCL.argc);
	for (int i=0; i<ZpZCL.argc; i++)
	{
		filelist[i] = (char *) malloc ((sizeof (char)) * PATH_MAX);
		for (int j=0; j<PATH_MAX; j++)
			filelist[i][j] = 0;
	}
	for (int i=0; i<ZpZCL.argc; i++)
		strcpy(filelist[i], filepath_vector[i].c_str());
	ZpZCL.FNV = (char **) filelist;  /* list of files to archive */

	res = (*ZipInit)(&lpZipUserFunctions);
	if (!res)
		return -1;

	res = ZipSetOptions(&ZpOpt);
	
	res = ZipArchive(ZpZCL);
	if (res  != 0)
		return -1;

	// cleanup
	for (int i=0; i<ZpZCL.argc; i++)
		free(filelist[i]);
	free(filelist);

	for(int i=0; i< (int) tempfiles.size(); i++)
	{
		remove(tempfiles[i].c_str());
	}
	rmdir(TEMPDIR);

	FreeLibrary(hZipDll);


	return 0;	
}

Conditioner::Register< PackagerProxy > registerObject;
