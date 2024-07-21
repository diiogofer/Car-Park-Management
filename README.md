# Automated Car Parking Management System 
(Introduction to Data Structures and Algorithms 2024 - Project) 

Developed a comprehensive software system for managing car parking lots, incorporating features such as automated entry/exit, real-time slot availability, and payment processing.

The project brief is available in [ProjectRules.md](ProjectRules.md).
This project resolution was awarded the highest score as it was considered an exemplary use of the techniques taught in the course. </br>


**Important notes:**

- Compilation process using gcc (version 12.3.0):
```
gcc -O3 -Wall -Wextra -Werror -Wno-unused-result -o proj1 *.c
```

- After compiling the project, to run the public tests, you should unzip and enter the test directory:
```
unzip public-tests.zip
```
```
cd public-tests
```
- To run all the public tests using the provided Makefile, you should execute the following command within the public-tests directory:
```
make
```
### How to Use the Car Parking Management Software
#### Starting the Software
After compiling the project as described above, you can start the software by running the following command in the terminal:
```
./proj1
```
After this, you will be constantly prompted for input into the command terminal.

Terminate the Program:

 - Command 'q' </br>
  Description: Terminate the Program </br>
  Usage Example:
```
q
```

 - Command p  (name capacity value-15min value-15-after-1-hour max-daily-value) </br>
 Description: Create or List Parking Lots </br>
 Usage Examples: </br>

To list all parking lots:
```
p
```
To create a new parking lot:
```
p Saldanha 200 0.20 0.30 12.00
```
or
```
p "CC Colombo" 400 0.25 0.40 20.00
```

 - Command: e (parking-name license-plate date time) </br>
 Description: Register Vehicle Entry </br>
 Usage Example: </br>
```
e Saldanha AA-00-AA 01-03-2024 08:34
```

 - Command: s (parking-name license-plate date time) </br>
 Description: Register Vehicle Exit </br>
 Usage Example:</br>
```
s Saldanha AA-00-AA 01-03-2024 10:59
```

 - Command: v (license-plate) </br>
 Descripton: List Vehicle Entries and Exits </br>
 Usage Example: </br>
```
v AA-00-AA
```

 - Command: f parking-name (date) </br>
 Description: Show Parking Lot Revenue </br>
 To show daily revenue summary: </br>
```
f Saldanha
```
To show detailed revenue for a specific day:
```
f Saldanha 01-03-2024
```

 - Command: r <parking-name>
 Description: Remove Parking Lot
 Usage Example:
```
r "CC Colombo"
```

#### Error Messages and Explanations
##### General Errors:
 - "parking already exists." ->  if the parking name already exists.
 - invalid capacity. if the capacity is less than or equal to 0.
 - invalid cost. if one of the costs is less than or equal to 0 or if the tariff values are not increasing.
 - too many parks. if the number of parks created is at its limit.
##### Entry Errors:
 - no such parking. if the parking name does not exist.
 - parking is full. if the parking lot is full.
 - invalid licence plate. if the license plate is invalid.
 - invalid vehicle entry. if the car is already inside a parking lot.
 - invalid date. if the date or time is invalid or before the last recorded entry or exit.
##### Exit Errors:
 - no such parking. if the parking name does not exist.
 - invalid licence plate. if the license plate is invalid.
 - invalid vehicle exit. if the car is not inside the specified parking lot.
 - invalid date. if the date or time is invalid or before the last recorded entry or exit.
##### Revenue Errors:
 - no such parking. if the parking name does not exist.
 - invalid date. if the date is invalid or after the last recorded entry or exit.
##### Accepted License Plate Format
A vehicle's license plate must be in the following format: </br>
- Three pairs of characters separated by a hyphen ('-').
- Each pair can either contain uppercase letters (A-Z) or decimal digits (0-9).
- A pair cannot contain both a letter and a digit.
- A license plate must contain at least one pair of letters and at least one pair of digits.
Example: AA-00-AA.

#### Contributions
Contributions are welcome! Feel free to open an issue or submit a pull request with improvements and fixes.


