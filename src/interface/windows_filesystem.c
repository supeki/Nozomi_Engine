// Nozomi Engine
// windows filesystem backend
// windows_filesystem.c

#include "../game_defs.h"
#include "../i_system.h"

#include <windows.h>

#ifdef WINCE

static void I_WideToChar(const WCHAR *src, char *dst, int dstlen)
{
    WideCharToMultiByte(
        CP_ACP,
        0,
        src,
        -1,
        dst,
        dstlen,
        NULL,
        NULL
    );
}

static void I_CharToWide(const char *src, WCHAR *dst, int dstlen)
{
    MultiByteToWideChar(
        CP_ACP,
        0,
        src,
        -1,
        dst,
        dstlen
    );
}

#endif

const char *I_GetHomeDir(void)
{
	#ifdef WINCE
    static char path[MAX_PATH];
    WCHAR wpath[MAX_PATH];
    char *slash;
    DWORD len;

    len = GetModuleFileName(NULL, wpath, MAX_PATH);

    if (len == 0)
        return ".";

    I_WideToChar(wpath, path, MAX_PATH);

    slash = strrchr(path, '\\');

    if (slash != NULL)
        *slash = '\0';

    return path;
	#else
    static char path[MAX_PATH];

    if (GetCurrentDirectoryA(MAX_PATH, path) == 0)
        return ".";

    return path;
#	endif
}

dirfiles_t I_GetDir(const char *path)
{
    dirfiles_t dirfiles;
    uint32_t i = 0, j = 0;

	#ifdef WINCE
    WIN32_FIND_DATA finddata;
    WCHAR searchpath[MAX_PATH];
	#else
    WIN32_FIND_DATAA finddata;
    char searchpath[MAX_PATH];
	#endif
	HANDLE find;

    // init dirfiles to prevent issues
    memset(&dirfiles, 0, sizeof(dirfiles_t));

	#ifdef WINCE

    // create search path
    I_CharToWide(path, searchpath, MAX_PATH - 2);
    wcscat(searchpath, L"\\*");

    find = FindFirstFile(searchpath, &finddata);
    if (find == INVALID_HANDLE_VALUE) // invalid path
        return dirfiles;

    while (1) {
        char filename[MAX_PATH];

        I_WideToChar(
            finddata.cFileName,
            filename,
            MAX_PATH
        );

        if (!strcmp(filename, ".") || !strcmp(filename, ".."))
            goto next_entry; // don't include relative dirs

        if (finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            dirfiles.num_dirs++;
        else
            dirfiles.num_files++;

		next_entry:
        if (!FindNextFile(find, &finddata))
            break;
    }

	#else

    // create search path
    sprintf(searchpath, "%s\\*", path);

    find = FindFirstFileA(searchpath, &finddata);
    if (find == INVALID_HANDLE_VALUE) // invalid path
        return dirfiles;

    while (1) {
        if (!strcmp(finddata.cFileName, ".") ||
            !strcmp(finddata.cFileName, ".."))
            goto next_entry; // don't include relative dirs

        if (finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            dirfiles.num_dirs++;
        else
            dirfiles.num_files++;

		next_entry:
        if (!FindNextFileA(find, &finddata))
            break;
    }

	#endif

    FindClose(find);

    if ((dirfiles.num_files + dirfiles.num_dirs) == 0) // no dirs or files
        return dirfiles;

    // allocate files
    if (dirfiles.num_files > 0) {
        dirfiles.filenames =
            malloc(dirfiles.num_files * sizeof(*dirfiles.filenames));

        if (dirfiles.filenames == NULL) // no memory
            dirfiles.num_files = 0;
    }

    // allocate dirs
    if (dirfiles.num_dirs > 0) {
        dirfiles.directories =
            malloc(dirfiles.num_dirs * sizeof(*dirfiles.directories));

        if (dirfiles.directories == NULL) // no memory
            dirfiles.num_dirs = 0;
    }

	#ifdef WINCE

    find = FindFirstFile(searchpath, &finddata);
    if (find == INVALID_HANDLE_VALUE) { // invalid directory...??
        if (dirfiles.filenames != NULL) {
            free(dirfiles.filenames);
            dirfiles.filenames = NULL;
        }

        if (dirfiles.directories != NULL) {
            free(dirfiles.directories);
            dirfiles.directories = NULL;
        }

        dirfiles.num_files = dirfiles.num_dirs = 0;
        return dirfiles;
    }

    while (1) {
        char filename[MAX_PATH];
        size_t len;

        I_WideToChar(
            finddata.cFileName,
            filename,
            MAX_PATH
        );

        if (!strcmp(filename, ".") || !strcmp(filename, ".."))
            goto next_file; // don't include relative dirs

        len = strlen(filename);

        if (finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (dirfiles.directories == NULL)
                goto next_file;

            // allocate dirname
            dirfiles.directories[j] = malloc(len + 1);

            if (dirfiles.directories[j] == NULL) // no memory
                break;

            strcpy(dirfiles.directories[j], filename);
            j++;
        } else {
            if (dirfiles.filenames == NULL)
                goto next_file;

            // allocate filename
            dirfiles.filenames[i] = malloc(len + 1);

            if (dirfiles.filenames[i] == NULL) // no memory
                break;

            strcpy(dirfiles.filenames[i], filename);
            i++;
        }

		next_file:
        if (!FindNextFile(find, &finddata))
            break;
    }

	#else

    find = FindFirstFileA(searchpath, &finddata);
    if (find == INVALID_HANDLE_VALUE) { // invalid directory...??
        if (dirfiles.filenames != NULL) {
            free(dirfiles.filenames);
            dirfiles.filenames = NULL;
        }

        if (dirfiles.directories != NULL) {
            free(dirfiles.directories);
            dirfiles.directories = NULL;
        }

        dirfiles.num_files = dirfiles.num_dirs = 0;
        return dirfiles;
    }

    while (1) {
        size_t len;

        if (!strcmp(finddata.cFileName, ".") ||
            !strcmp(finddata.cFileName, ".."))
            goto next_file; // don't include relative dirs

        len = strlen(finddata.cFileName);

        if (finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (dirfiles.directories == NULL)
                goto next_file;

            // allocate dirname
            dirfiles.directories[j] = malloc(len + 1);

            if (dirfiles.directories[j] == NULL) // no memory
                break;

            strcpy(dirfiles.directories[j], finddata.cFileName);
            j++;
        } else {
            if (dirfiles.filenames == NULL)
                goto next_file;

            // allocate filename
            dirfiles.filenames[i] = malloc(len + 1);

            if (dirfiles.filenames[i] == NULL) // no memory
                break;

            strcpy(dirfiles.filenames[i], finddata.cFileName);
            i++;
        }

		next_file:
        if (!FindNextFileA(find, &finddata))
            break;
    }

	#endif

    FindClose(find);

    dirfiles.num_files = i; // if an allocation failed, still get the rest of the files
    dirfiles.num_dirs = j; // same thing for dirs

    return dirfiles;
}