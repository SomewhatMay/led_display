#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

#define GPIO1_BASE      0x020A0000
#define GPIO_SIZE       0x1000
#define GPIO1_DR        0x00
#define GPIO1_GDIR      0x04

#define GPIO_PIN        (1 << 25)  // GPIO1_IO25 is pin 25 on GPIO1 (i.e., GPIO 57)

int main() {
    int fd;
    volatile uint32_t *gpio1;

    // Open /dev/mem
    fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // Memory-map GPIO1
    gpio1 = (volatile uint32_t *) mmap(NULL, GPIO_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO1_BASE);
    if (gpio1 == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    // Set GPIO1_IO25 as output
    gpio1[GPIO1_GDIR / 4] |= GPIO_PIN;

    // Blink loop
    for (int i = 0; i < 10; i++) {
        gpio1[GPIO1_DR / 4] |= GPIO_PIN;   // Set pin high
        printf("HIGH");
        usleep(500000);                   // 500 ms
        gpio1[GPIO1_DR / 4] &= ~GPIO_PIN; // Set pin low
        printf("LOW");
        usleep(500000);
    }

    // Cleanup
    munmap((void *) gpio1, GPIO_SIZE);
    close(fd);
    return 0;
}
