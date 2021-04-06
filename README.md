# MPIPasswordCracker
Using MPI to crack hashed passwords

void increment_numerical_string(std::string& s)
{
    std::string::reverse_iterator iter = s.rbegin(), end = s.rend();
    int carry = 1;
    while (carry && iter != end)
    {
        int value = (*iter - '0') + carry;
        carry = (value / 10);
        *iter = '0' + (value % 10);
        ++iter;
    }
    if (carry)
        s.insert(0, "1");
}
