#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "must provide one positional argument: memory in megabytes\n");
        return 1;
    }

    int amountInMb;
    int result = sscanf(argv[1], "%d", &amountInMb);
    if (result == EOF)
    {
        fprintf(stderr, "invalid integer for amount of memory in megabytes\n");
        return 1;
    }

    printf("will reserve %d megabytes of memory\n", amountInMb);

    int kbInMb = 1024;
    int bytesInKb = 1024;

    int blockSizeInMb = 128;
    size_t blockSizeInBytes = blockSizeInMb * kbInMb * bytesInKb;

    printf("using block size of %d megabytes\n", blockSizeInMb);

    int numBlocks = amountInMb / blockSizeInMb;

    int excessBlockSizeInMb = amountInMb % blockSizeInMb;
    size_t excessBlockSizeInBytes = excessBlockSizeInMb * kbInMb * bytesInKb;

    printf("allocating %d blocks of %d megabytes\n", numBlocks, blockSizeInMb);

    int urandomFd = open("/dev/urandom", O_RDONLY);

    void *blocks[numBlocks];
    for (int i = 0; i < numBlocks; i++)
    {
        if (i != 0)
        {
            // Clear last line
            printf("\x1b[1F\x1b[2K");
        }

        printf("allocating block %d of %d\n", i + 1, numBlocks);

        blocks[i] = calloc(1, blockSizeInBytes);
        read(urandomFd, blocks[i], blockSizeInBytes);
        mlock(blocks[i], blockSizeInBytes);
    }

    void *excessBlock;
    if (excessBlockSizeInMb > 0)
    {
        printf("allocating excess of %d megabytes\n", excessBlockSizeInMb);
        excessBlock = calloc(1, excessBlockSizeInBytes);
        read(urandomFd, excessBlock, excessBlockSizeInBytes);
        mlock(excessBlock, excessBlockSizeInBytes);
    }

    close(urandomFd);

    printf("holding memory\n");
    printf("press the ENTER key to release memory and exit\n");
    getchar();

    printf("releasing memory\n");
    for (int i = 0; i < numBlocks; i++)
    {
        munlock(blocks[i], blockSizeInBytes);
        free(blocks[i]);
    }

    if (excessBlockSizeInMb > 0)
    {
        munlock(excessBlock, blockSizeInBytes);
        free(excessBlock);
    }

    printf("all memory released\n");

    return 0;
}
