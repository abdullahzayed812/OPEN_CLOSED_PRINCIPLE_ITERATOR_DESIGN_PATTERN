/**
 * This code just i will learn two two things
 * 1- Open-Closed principle
 * "Software entities ... should be open for extension, but closed for
 * modification", i managed to extend the functionality of the class to one
 * controlled printing, i didn't need to change original code
 *
 * 2- Iterator Design Pattern
 * HasNext & GetNext is a general strategy to allow iterating on something.
 * STL has iterator on several containers
 */

#include <cctype>
#include <iostream>
#include <utility>
#include <vector>

class StudentGradesInfo {
 private:
  std::string studentId;
  std::vector<double> grades;
  std::vector<std::string> coursesNames;

  const double MAX_GRADE_PER_COURSE = 100.0;

  static int statisticsTotalPrints;

  std::string toLowerCase(const std::string& str) const {
    std::string result;

    for (int i = 0; i < str.size(); i++) {
      result += tolower(str[i]);
    }

    return result;
  }

  double adjustGrade(double grade) const {
    if (grade < 0) {
      return 0;
    } else if (grade > this->MAX_GRADE_PER_COURSE) {
      return this->MAX_GRADE_PER_COURSE;
    }

    return grade;
  }

 public:
  StudentGradesInfo(std::string id) : studentId(id) {}

  bool addGrade(double grade, const std::string& courseName) {
    grade = this->adjustGrade(grade);

    for (int i = 0; i < (int)this->coursesNames.size(); i++) {
      if (this->toLowerCase(courseName) ==
          this->toLowerCase(this->coursesNames[i])) {
        return false;
      }
    }

    this->coursesNames.push_back(courseName);
    this->grades.push_back(grade);

    return true;
  }

  void printAllCourses() const {
    StudentGradesInfo::statisticsTotalPrints++;

    std::cout << "Grades for student id: " << this->studentId << "\n";

    for (int i = 0; i < (int)this->coursesNames.size(); i++) {
      std::cout << "\t Course name: " << coursesNames[i]
                << "\t Grade: " << this->grades[i] << "\n";
    }
  }

  bool getCourseGradesInfo(int pos,
                           std::pair<std::string, double>& gradesInfo) const {
    if (pos < 0 || pos >= (int)this->coursesNames.size()) {
      gradesInfo = std::make_pair("", -1);
      return false;
    }

    gradesInfo = std::make_pair(this->coursesNames[pos], this->grades[pos]);
    return true;
  }

  std::pair<double, double> getTotalGradesSum() const {
    double sum = 0, total = 0;

    for (int i = 0; i < (int)this->coursesNames.size(); i++) {
      sum += this->grades[i];
      total += this->MAX_GRADE_PER_COURSE;
    }

    return std::make_pair(sum, total);
  }

  std::string getStudentId() const { return this->studentId; }

  int getTotalCoursesCount() const { return this->coursesNames.size(); }
};

class StudentGradesInfoPrinter {
 private:
  const StudentGradesInfo& studentInfo;
  int coursesCount;

 public:
  StudentGradesInfoPrinter(const StudentGradesInfo& student)
      : studentInfo(student), coursesCount(0) {}

  bool hasNext() {
    return this->studentInfo.getTotalCoursesCount() > this->coursesCount;
  }

  std::pair<std::string, double> getNext() {
    std::pair<std::string, double> result;

    this->studentInfo.getCourseGradesInfo(this->coursesCount, result);
    this->coursesCount++;

    return result;
  }

  void resetIterator() { this->coursesCount = 0; }
};

int main() {
  StudentGradesInfo student("S001");
  StudentGradesInfoPrinter printer(student);

  student.addGrade(80, "Math");
  student.addGrade(90, "Programming 1");

  int limit = 3;

  std::cout << "Printing top " << limit << " grades if available\n";

  while (limit-- && printer.hasNext()) {
    std::pair<std::string, double> result = printer.getNext();

    std::cout << "\t " << result.first << " = " << result.second << "\n";
  }

  student.addGrade(30, "OOP");
  student.addGrade(30, "Data structures & Algorithms");

  printer.resetIterator();

  limit = 3;

  std::cout << "Printing top " << limit << " grades if available\n";

  while (limit-- && printer.hasNext()) {
    std::pair<std::string, double> result = printer.getNext();

    std::cout << "\t " << result.first << " = " << result.second << "\n";
  }

  return 0;
}
