#include "manager.h"

int main() {

    Manager m;
    m.detectAndSelfAssign();
    m.waitConnect();
    m.runPreview();

    return 0;
}
