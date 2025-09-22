# C++98 Menu-Driven Learning Management System (LMS)

A simple, robust terminal-based LMS for students, teachers, and courses. Written fully in C++98 and runs in any terminal/command prompt with basic libraries.

---

## 📚 Description

This program helps you manage student, teacher, and course data for a school or class, including marks and rankings. It uses easy-to-follow menus and guides you step by step with clear instructions. No prior programming experience is needed to use it!

---

## 💻 Features

- Add, view, update, and search students
- Add, view, update, and search teachers
- Add, view, update, and search courses
- Enroll students in courses
- Enter and update marks for students
- Calculate and display GPA
- Rank students by course marks or overall GPA
- Search by full name, ID, or partial name
- Bulk add students and courses
- Handles all input errors without crashing

---

## 🛠️ Requirements

- Windows, Linux, or Mac with a C++ compiler (`g++`, Dev-C++, Code::Blocks, or similar)
- Runs in the terminal/command prompt only (no graphics)
- Completed and saved file named `main.cpp`

---

## 🚀 How to Compile

Open your terminal in the project folder and type:
g++ -std=c++98 main.cpp -o lms
This will create an executable called `lms` (or `lms.exe` on Windows).

---

## ▶️ How to Run

- On Windows:  
  Double-click `lms.exe` or type in terminal:  
lms.exe
- On Mac/Linux:  
In terminal, type:  
./lms

---

## 📝 How to Use

- The program shows menus with numbers for each option—type a number and press Enter.
- Answer each prompt shown on screen (name, ID, marks, etc.).
- Press `0` to go back to the previous menu or exit.
- When updating, you can press Enter to skip updating a field.

**If you enter text where a number is expected, the program will warn you and ask again.**

---

## 📋 Example Tasks

- **Add a new student:**  
Go to Student Management > Add Student, and fill in all prompts.
- **Search for a student:**  
Student Management > Search Student, then type the name or ID.
- **Bulk add students or courses:**  
Use the Bulk Add option in respective menus.
- **View rankings:**  
Select Ranking & GPA from the main menu.

---

## ℹ️ Notes & Limitations

- Your data is held in memory until you close the program; it does not save between sessions or use files/databases.
- The interface is text-only (no color or graphics).
- Designed for demonstration or small project, not large-scale deployment.

---

## 🧑‍💻 FAQ

**Q: What do I do if I type the wrong thing?**  
A: The program will show a warning and let you enter your choice again.

**Q: How do I exit or go back?**  
A: Enter `0` at any menu.

**Q: What if I want to leave a field unchanged when updating info?**  
A: Just press Enter—leaving it blank keeps the old value.

---

## 📖 License & Credits

- Free for educational, personal, class, and homework use!
- Developed for BCA project demonstration and C++ OOP practice.

---

*This project uses only C++98 features for maximum compatibility.*
