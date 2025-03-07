#include <stdio.h>
#include "AIchat_c_interface.h"

int main() {
    void* app = create_application("192.168.211.1", 8765, "123456", "00:11:22:33:44:55", "sk-xxxx", 1, 16000, 1, 40);
    if (app == NULL) {
        printf("Failed to create application.\n");
        return -1;
    }

    run_application(app);

    destroy_application(app);

    return 0;
}