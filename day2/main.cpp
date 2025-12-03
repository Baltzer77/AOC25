#include <iostream>
#include <string>
#include <vector>
#include <inttypes.h>
#include <math.h>
#include <unordered_set>

int64_t find_initial_value(const std::string &current, int amount)
{
    if ((current.size() % amount) != 0)
    {
        int halfway = (current.size() / amount);

        return std::powl(10, halfway);
    }

    // first half of string
    int nextStep = current.size() / amount;

    int64_t first_part = std::stoll(current.substr(0, nextStep));
    for (int i = 1; i < amount; i++)
    {
        std::string part = current.substr(i * nextStep, nextStep);
        int64_t part_value = std::stoll(part);
        if (part_value > first_part)
        {
            return first_part + 1;
        }
        else if (part_value < first_part)
        {
            return first_part;
        }
    }
    return first_part;
}

int main()
{
    std::string line;

    getline(std::cin, line);

    std::vector<std::pair<std::string, int64_t>> ranges;

    size_t pos = 0;
    while (pos < line.length())
    {
        int dash = line.find('-', pos);
        std::string left = line.substr(pos, dash - pos);
        std::string token = line.substr(0, pos);

        pos = line.find(',', dash);
        if (pos == std::string::npos)
        {
            pos = line.length();
        }
        std::string right = line.substr(dash + 1, pos - dash - 1);
        ranges.push_back({left, std::stoll(right)});
        pos++;
    }

    int64_t sum = 0;

    for (const auto &[left, right] : ranges)
    {
        std::unordered_set<int64_t> seen;
        for (int multiplier = 2; multiplier <= 9; multiplier++)
        {
            int64_t current_value = find_initial_value(left, multiplier);

            while (true)
            {
                std::string current = std::to_string(current_value);

                std::string actual = "";
                for (int i = 0; i < multiplier; i++)
                {
                    actual += current;
                }

                int64_t doubled_value = std::stoll(actual);
                if (doubled_value > right)
                {
                    break;
                }

                if (seen.find(doubled_value) != seen.end())
                {
                    current_value++;
                    continue;
                }
                seen.insert(doubled_value);

                //std::cout << "Invalid: " << doubled_value << std::endl;
                sum += doubled_value;
                
                current_value++;
            }
        }
    }

    std::cout << sum << std::endl;
    return 0;
}