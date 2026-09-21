// Nozomi Engine
// Commodore Amiga backend
// amiga_filesystem.c

#include "../../game_defs.h"
#include "../../i_system.h"

#include <exec/types.h>
#include <dos/dos.h>
#include <dos/dosextens.h>

#include <proto/exec.h>
#include <proto/dos.h>

const char *I_GetHomeDir(void)
{
    return "\0"; // simply just, return nothing.
}

dirfiles_t I_GetDir(const char *path)
{
	dirfiles_t dirfiles;
	BPTR lock;
	struct FileInfoBlock fib;
	uint32_t i = 0, j = 0;

	memset(&dirfiles, 0, sizeof(dirfiles_t));

	lock = Lock((STRPTR)path, ACCESS_READ);

	if (lock == 0)
		return dirfiles;

	if (!Examine(lock, &fib))
	{
		UnLock(lock);
		return dirfiles;
	}

	while (ExNext(lock, &fib))
	{
		if (fib.fib_DirEntryType > 0)
			dirfiles.num_dirs++;
		else
			dirfiles.num_files++;
	}

	UnLock(lock);

	if (dirfiles.num_files > 0)
	{
		dirfiles.filenames = malloc(dirfiles.num_files * sizeof(*dirfiles.filenames));

		if (dirfiles.filenames == NULL)
			dirfiles.num_files = 0;
	}

	if (dirfiles.num_dirs > 0)
	{
		dirfiles.directories = malloc(dirfiles.num_dirs * sizeof(*dirfiles.directories));

		if (dirfiles.directories == NULL)
			dirfiles.num_dirs = 0;
	}

	if ((dirfiles.num_files + dirfiles.num_dirs) == 0)
		return dirfiles;

	lock = Lock((STRPTR)path, ACCESS_READ);

	if (lock == 0)
	{
		DF_Free(&dirfiles);
		return dirfiles;
	}

	if (!Examine(lock, &fib))
	{
		UnLock(lock);
		DF_Free(&dirfiles);
		return dirfiles;
	}

	while (ExNext(lock, &fib))
	{
		size_t len = strlen(fib.fib_FileName);

		if (fib.fib_DirEntryType > 0)
		{
			if (dirfiles.directories == NULL)
				continue;

			dirfiles.directories[j] = malloc(len + 1);

			if (dirfiles.directories[j] == NULL)
				break;

			strcpy(dirfiles.directories[j], fib.fib_FileName);
			j++;
		}
		else
		{
			if (dirfiles.filenames == NULL)
				continue;

			dirfiles.filenames[i] = malloc(len + 1);

			if (dirfiles.filenames[i] == NULL)
				break;

			strcpy(dirfiles.filenames[i], fib.fib_FileName);
			i++;
		}
	}

	UnLock(lock);

	dirfiles.num_files = i;
	dirfiles.num_dirs = j;

	return dirfiles;
}