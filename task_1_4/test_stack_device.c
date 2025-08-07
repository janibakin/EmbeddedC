#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main() {
    int fd, value;

    // open the device file
    fd = open("/dev/stackdev", O_RDWR);
    if (fd < 0) {
        perror("Failed to open /dev/stackdev");
        return 1;
    }

    // add numbers 1-17 to the stack by writing
    for (int i = 1; i <= 17; ++i) {
        if (write(fd, &i, sizeof(int)) < 0) {
            perror("Write error");
            break;
        } else {
            printf("Pushed %d\n", i);
        }
    }

    // remove numbers from the stack by reading
    for (int i = 0; i < 17; ++i) {
        if (read(fd, &value, sizeof(int)) < 0) {
            perror("Read error");
            break;
        } else {
            printf("Popped %d\n", value);
        }
    }

    // close the device file
    if (close(fd) < 0) {
        perror("Failed to close /dev/stackdev");
        return 1;
    }
    printf("Device operations completed successfully.\n");
    return 0;
}