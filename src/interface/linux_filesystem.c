// Nozomi Engine
// linux filesystem backend
// linux_filesystem.c

#include "../game_defs.h"
#include "../i_system.h"

#include <dirent.h>
#include <sys/stat.h>

const char *I_GetHomeDir(void)
{
    // waow nds and psp support too
    #if defined(__NDS__)
    return "./";
    #elif defined(PSP)
    return va("ms0:/PSP/GAME/%s/", GAME_NAME);
    #else
    const char *home = getenv("HOME");

    if (home == NULL)
        home = ".";

    return home;
    #endif
}

dirfiles_t I_GetDir(const char *path)
{
    dirfiles_t dirfiles;
    DIR *dir = opendir(path);
    struct dirent *entry;
    uint32_t i = 0, j = 0;

    // init dirfiles to prevent issues
    memset(&dirfiles, 0, sizeof(dirfiles_t));

    if (dir == NULL) // invalid path
        return dirfiles;

    while ((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue; // don't include relative dirs

        if (entry->d_type == DT_DIR)
            dirfiles.num_dirs++;
        else if (entry->d_type == DT_REG)
            dirfiles.num_files++;
    }

    closedir(dir);
    if ((dirfiles.num_files + dirfiles.num_dirs) == 0) // no dirs or files
        return dirfiles;

    // allocate files
    dirfiles.filenames = malloc(dirfiles.num_files * sizeof(*dirfiles.filenames));
    if (dirfiles.filenames == NULL) // no memory
        dirfiles.num_files = 0;

    // allocate dirs
    dirfiles.directories = malloc(dirfiles.num_dirs * sizeof(*dirfiles.directories));
    if (dirfiles.directories == NULL) // no memory
        dirfiles.num_dirs = 0;

    dir = opendir(path);
    if (dir == NULL) { // invalid directory...??
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
    
    while ((entry = readdir(dir)) != NULL) {
        size_t len;

        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue; // don't include relative dirs

        len = strlen(entry->d_name);
        if (entry->d_type == DT_DIR) {
            if (dirfiles.directories == NULL)
                continue;

            // allocate dirname
            dirfiles.directories[j] = malloc(len + 1);
            if (dirfiles.directories[j] == NULL) // no memory
                break;

            strcpy(dirfiles.directories[j], entry->d_name);
            j++;
        } else if (entry->d_type == DT_REG) {
            if (dirfiles.filenames == NULL)
                continue;

            // allocate filename
            dirfiles.filenames[i] = malloc(len + 1);
            if (dirfiles.filenames[i] == NULL) // no memory
                break;

            strcpy(dirfiles.filenames[i], entry->d_name);
            i++;
        }
    }

    closedir(dir);
    dirfiles.num_files = i; // if an allocation failed, still get the rest of the files
    dirfiles.num_dirs = j; // same thing for dirs

    return dirfiles;
}