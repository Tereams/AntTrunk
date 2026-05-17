#include <optional>

enum class Frequency {
    Daily,
    Weekly,
    Biweekly,
    Monthly,
    Yearly
};

enum class Weekday {
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
};

struct RecurrenceRule {
    Frequency frequency = Frequency::Monthly;

    // Example:
    // interval = 1 with Monthly means every month.
    // interval = 2 with Weekly means every two weeks.
    int interval = 1;

    // Used for monthly recurrence, such as the 15th of every month.
    std::optional<int> day_of_month;

    // Used for weekly or biweekly recurrence, such as every Friday.
    std::optional<Weekday> day_of_week;
};
