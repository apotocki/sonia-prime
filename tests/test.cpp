size_t operator "" _a0(const char* str, size_t v) {
    return v;
}

void do_nothing(size_t) {}

void foo_ok(int arg) {
    size_t r = "string"_a0; // OK
    do_nothing("string"_a0); // OK
}

template <typename T>
void foo_fail() {
    size_t r = "string"_a0; // OK
    do_nothing("string"_a0); // error
}

int main() { return 0; }