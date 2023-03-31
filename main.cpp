#include "manager.h"

int main() {

    Manager m;
    m.waitConnect();
    m.run();

    return 0;
}
