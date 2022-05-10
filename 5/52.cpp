template<int first, int second>
void f()
{
    const long long n = static_cast<long long>(first) - second - 1;
    int arr[n < 0 ? -1 : 1];

    //without this part: compilation error - unused arr
    arr[0] = 1;
    int v = arr[0];
    v += 2;
}
