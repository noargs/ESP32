#include <stdio.h>
#include <string.h>

struct Person
{
  char first_name[20];
  char last_name[20];
  int age;
  void (*DoWork)(char *dataRetrieved);
};

void updatePerson(struct Person *person, char *dataRetrieved)
{
  strcpy(person->first_name, "data from dataRetrieved");
  strcpy(person->last_name, "data from dataRetrieved");
  person->DoWork = doWorkForPerson;
  person->DoWork("Some params");
  person->age = 23;
}

void connectAndGetInfo(char *url, void (*DoWork)(char *dataRetrieved))
{
  // connect to wireless
  // connect to endpoint
  //allocate memory
  char *dataRetrieved = (char*)malloc(1024);
  // fill buffer with data: getRequest(&dataRetrieved);
  // do work and get a person object
  DoWork(dataRetrieved);
  // clean up memory and other resources
  free((void*)dataRetrieved);
}

void doWorkForPerson(char *dataRerieved)
{
  struct Person person;
  updatePerson(&person, dataRerieved);
  printf("person: %s %s is %d years old\n", person.first_name, person.last_name, person.age);
}

void app_main(void)
{
  connectAndGetInfo("http://getperson.com", doWorkForPerson);
}
