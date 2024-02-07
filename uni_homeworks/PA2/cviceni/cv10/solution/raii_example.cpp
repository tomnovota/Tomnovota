#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

void func_throw(const char* what)
{
    using namespace std::string_literals;
    throw std::runtime_error("Throw via "s + what);
}

void dangerous()
{
    FILE* fp = fopen("test.txt", "w");
    int* arr = new int[5];

    /* no resource cleanup after throw and stack unwinding
     * dtors are called but neither FILE* dtor nor int* dtor cleans up the resources they manage.
     * They are simple pointers destructed by "not doing anything" (compiler can't
     * possibly know if this ptr was the one that "holds and manages" the resource.
     * So neither fclose nor delete[] is called!
     */
    func_throw("dangerous"); // 2 mem leaks (check with valgrind --leak-check-full)

    delete[] arr;
    fclose(fp);
}

void file_deleter(FILE* ptr)
{
    fclose(ptr);
}

void better()
{
    auto* fp = fopen("test.txt", "w");
    std::unique_ptr<FILE, decltype(&file_deleter)> fp_manager(fp, file_deleter);

    // let unique_ptr manage array so we don't have to call delete[]
    std::unique_ptr<int[]> arr = std::make_unique<int[]>(5);

    /*
     * dtors are called when this func throws and stack unwinding occurs
     * unique_ptr<FILE*, decltype(file_deleter)> has dtor that handles resource free
     * unique_ptr<int[]> same
     */
    func_throw("better");
}

int main()
{
    try {
        better();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    try {
        dangerous();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
