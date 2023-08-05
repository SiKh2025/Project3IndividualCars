#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <list>

//structure to define Car categories given data set
struct Car {
    //all  in order of data set
    //def height, length, and width
    int height, length, width;
    //def driveline and engine type
    std::string driveline, engineType;
    //def for if car hybrid or not
    bool hybrid;
    //def for  number of gears
    int numOfGears;
    //def for type of transmission
    std::string transmission;
    //def fuel efficiency for city
    int cityMpg;
    //def fuel type
    std::string fuelType;
    //def fuel efficiency for highway
    int highwayMpg;
    //def car classification (compact, sedan, SUV), unique ID, make (manufacturer), and model year
    std::string classification, id, make, modelYear;
    //def car model year(dif than string rep), horsepower, and torque
    int year, horsepower, torque;
};

//Def size of the hash tables
//for over 100,000 data points
const int TABLE_SIZE = 142889;


//for Linear Probing (hash table)
//struct to def key-value pair to be stored in the linear hash table
struct LinearEntry {
    //def key (for hashing and retrieval)
    std::string key;
    Car value;
    //for if entry occupied
    bool occupied = false;
};

//static array of LinearEntry elements
//of hash table
LinearEntry linearHashTable[TABLE_SIZE];

//similar but for Separate Chaining (hash table)
//struct def ChainEntry (no occupied check needed)
struct ChainEntry {
    //def key (for hashing and retrieval)
    std::string key;
    Car value;
    //no occupied check, will auto do w/ empty nodes (linked lists)
};

//static array of ChainEntry elements
//of hash table
std::list<ChainEntry> chainHashTable[TABLE_SIZE];


//function converts a string key into an index in the hash table
int hashFunction(const std::string& key) {
    int hash = 0;
    //will iterates through each character of the string key
    for (char c : key) {
        //update hash table based on prev val + ASCII val of current char
        hash = (hash * 31 + c) % TABLE_SIZE;
    }
    return hash;
}
//for linear probing
//Insert function
void linearInsert(const std::string& key, const Car& value) {
    //to generate index for this key
    int index = hashFunction(key);
    //check if the spot is occupied will linearly probe to the next index
    while (linearHashTable[index].occupied) {
        index = (index + 1) % TABLE_SIZE;
    }
    //to store the key-value pair in the hash table
    linearHashTable[index].key = key;
    linearHashTable[index].value = value;
    //set the occupied flag to true
    linearHashTable[index].occupied = true;
}

//for linear probing
//Search function
Car* linearSearch(const std::string& key) {
    //to generate index for this key
    int index = hashFunction(key);
    //temp var to ref starting index
    int startIndex = index;
    //do-while loop for checking each spot in the hash table
    //start from the hashed index
    do {
        //check if the spot's key matches the search key
        if (linearHashTable[index].key == key) {
            //return a pointer to the value
            return &linearHashTable[index].value;
        }
        //if not, linearly probe to the next index
        index = (index + 1) % TABLE_SIZE;
    //continue while spot is occupied given the search not restarted to start index
    } while (linearHashTable[index].occupied && index != startIndex);
    //if key not found
    return nullptr;
}

//for Separate Chaining
//function to insert a key-value pair into the separate chaining hash table
void chainInsert(const std::string& key, const Car& value) {
    //hash the key to get an index
    int index = hashFunction(key);
    //temp: new ChainEntry to hold the key-value pair
    ChainEntry entry;
    entry.key = key;
    entry.value = value;
    //add the entry to the end of the chain
    chainHashTable[index].push_back(entry);
}

//function to search for a key in the separate chaining hash table
Car* chainSearch(const std::string& key) {
    //hash the key to get an index
    int index = hashFunction(key);
    //iterate through the chain at computed index
    for (auto& entry : chainHashTable[index]) {
        //If the current entry's key = the search key
        if (entry.key == key) {
            //return a pointer to value
            return &entry.value;
        }
    }
    //otherwise if key not found
    return nullptr;
}
//for both linear probing, separate chaining
//function to read data from a CSV file and insert to specfic hash table
//to read file
std::vector<Car> readCarsFromCSV(const std::string &filename, bool useLinearProbing) {
    //open file
    std::ifstream file(filename);
    //vector to store the cars data
    std::vector<Car> cars;
    std::string line, token;

    //test file not opening
    if (!file.is_open()) {
        std::cerr << "Could not open the file: " << filename << std::endl;
        return cars;
    }

    //to ignore the header row
    //cause this is just the titles of the categoreis of data
    std::getline(file, line);

    //process each line in the file
    while (std::getline(file, line)) {
        //use a stringstream to split the line into tokens
        std::stringstream ss(line);
        //temp: new car object
        //holds data for this line
        Car car;
        //Vector to hold tokens
        std::vector<std::string> columns;

        //to split the line into tokens based on commas (of file, sep data)
        while (std::getline(ss, token, ',')) {
            //to check if the token is enclosed in double quotes
            //so data is read correctly from file
            size_t start = token.find_first_of("\"");
            size_t end = token.find_last_of("\"");
            //if quotes remove them and add the token to the columns vector
            if (start != std::string::npos && end != std::string::npos) {
                columns.push_back(token.substr(start + 1, end - start - 1));
            //otherwise still add the token to the columns vector (no quotes)
            } else {
                columns.push_back(token);
            }
        }

        //so that all data of car inputed can be outputted
        //check if line does not have the correct number of columns (18)
        //then skip it
        if (columns.size() != 18) {
            continue;
        }

        //to parse the data from the columns into the Car object
        //of each of the categorries of the data from the file
        try {
            car.height = std::stoi(columns[0]);
            car.length = std::stoi(columns[1]);
            car.width = std::stoi(columns[2]);
            car.driveline = columns[3];
            car.engineType = columns[4];
            car.hybrid = (columns[5] == "True");
            car.numOfGears = std::stoi(columns[6]);
            car.transmission = columns[7];
            car.cityMpg = std::stoi(columns[8]);
            car.fuelType = columns[9];
            car.highwayMpg = std::stoi(columns[10]);
            car.classification = columns[11];
            car.id = columns[12];
            car.make = columns[13];
            car.modelYear = columns[14];
            car.year = std::stoi(columns[15]);
            car.horsepower = std::stoi(columns[16]);
            car.torque = std::stoi(columns[17]);

            //Add the Car to the vector of cars
            cars.push_back(car);
            //to insert the Car into the selected hash table via user
            //Linear Probing or Separate Chaining insert methods
            if (useLinearProbing) {
                linearInsert(car.id, car);
            } else {
                chainInsert(car.id, car);
            }
        //check if error from processing a line
        } catch (const std::invalid_argument &e) {
            //state error occured, skip line
            std::cerr << "Error occurred while processing line: " << line << "\nError: " << e.what() << std::endl;
            continue;
        }
    }
    return cars;
}

int main() {
    //file of refrence.
    const std::string filename = "cars.csv";
    //vector to hold all cars data from file.
    std::vector<Car> cars;

    //runs until user inputs terminate.
    while (true) {
        //user chooses hashing approach (Linear Probing or Separate Chaining)
        std::cout << "Choose Hashing Approach:\n";
        std::cout << "1. Linear Probing\n";
        std::cout << "2. Separate Chaining\n";
        std::cout << "Enter choice (1/2) or 'terminate' to quit: ";
        //input
        std::string choiceStr;
        std::getline(std::cin, choiceStr);

        //break to end program
        if (choiceStr == "terminate") {
            break;
        }

        //of user choice
        int choice = std::stoi(choiceStr);

        //of which method to use based on user choice
        switch (choice) {
            //if user chooses Linear Probing
            case 1:  // User chooses Linear Probing
                cars = readCarsFromCSV(filename, true);
                break;

            //if user chooses Separate Chaining
            case 2:
                cars = readCarsFromCSV(filename, false);
                break;
            //if input not 1 or 2
            default:
                std::cerr << "Invalid choice.\n";
                continue;
        }

        //display user to enter car ID or terminate
        std::cout << "\nEnter Identification.ID to search for (or 'terminate' to quit): ";
        //store input
        std::string testId;
        std::getline(std::cin, testId);

        //break to end program
        if (testId == "terminate") {
            break;
        }

        //to search for car from data based on user's choice of hashing
        Car* foundCar = (choice == 1) ? linearSearch(testId) : chainSearch(testId);

        //If car is found, print data from all categories (18)
        if (foundCar) {
            std::cout << "Found in hash map: \n";
            std::cout << "Height: " << foundCar->height << "\n";
            std::cout << "Length: " << foundCar->length << "\n";
            std::cout << "Width: " << foundCar->width << "\n";
            std::cout << "Driveline: " << foundCar->driveline << "\n";
            std::cout << "Engine Type: " << foundCar->engineType << "\n";
            std::cout << "Hybrid: " << (foundCar->hybrid ? "True" : "False") << "\n";
            std::cout << "Number of Gears: " << foundCar->numOfGears << "\n";
            std::cout << "Transmission: " << foundCar->transmission << "\n";
            std::cout << "City MPG: " << foundCar->cityMpg << "\n";
            std::cout << "Fuel Type: " << foundCar->fuelType << "\n";
            std::cout << "Highway MPG: " << foundCar->highwayMpg << "\n";
            std::cout << "Classification: " << foundCar->classification << "\n";
            std::cout << "ID: " << foundCar->id << "\n";
            std::cout << "Make: " << foundCar->make << "\n";
            std::cout << "Model Year: " << foundCar->modelYear << "\n";
            std::cout << "Year: " << foundCar->year << "\n";
            std::cout << "Horsepower: " << foundCar->horsepower << "\n";
            std::cout << "Torque: " << foundCar->torque << "\n";
        //otherwise if not found
        } else {
            std::cout << "Car with ID " << testId << " not found." << std::endl;
        }
    }

    return 0;
}



