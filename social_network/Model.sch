TABLE_(users)
    ISERIAL_     (id) PRIMARY_KEY
    STRING_      (first_name, 50) NOT_NULL
    STRING_      (last_name, 50) NOT_NULL
    DATE_        (birthday)
    STRING_      (gender, 10)
    STRING_      (interests, 200)
    STRING_      (city, 50)
    STRING_      (password, 200) NOT_NULL
END_TABLE