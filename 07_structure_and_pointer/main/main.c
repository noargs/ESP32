#include <stdio.h>
#include <string.h>

struct Person
{
  char first_name[20];
  char last_name[20];
  int age;
};

// void updatePerson(struct Person person)
// {
//   strcpy(person.first_name, "John");
//   strcpy(person.last_name, "Doe");
//   person.age = 25;      
// }

void updatePerson(struct Person *person)
{
  strcpy(person->first_name, "Bob");
  strcpy(person->last_name, "Fisher");
  person->age = 30;
}

void exclamIt(char *phrase)
{
  strcat(phrase, "!");
}

void app_main(void)
{
  
  char phrase[20] = {"Hello world"};
  exclamIt(phrase);
  printf("function output: %s\n", phrase);

  struct Person person;
//   updatePerson(person);  
  updatePerson(&person);
  printf("Person: %s %s is %d years old\n", person.first_name, person.last_name, person.age);
}