#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>


struct birthday 
{	
	int month;
	int day;
	int year;
	char gender;
        struct list_head list;
	char *name;
};

/**
 * The following defines and initializes a list_head object named birthday_list
 */
static LIST_HEAD(birthday_list);
struct birthday *student;
struct birthday *next;
struct birthday *youngest;


int simple_init(void)
{
       printk(KERN_INFO "Loading Module\n");

/*
 * The "youngest" variable is initialized here for use later...
 * ... to find the youngest student.
 */

       youngest = kmalloc(sizeof(*youngest), GFP_KERNEL);
       youngest->month = 0;
       youngest->day = 0;
       youngest->year = 1;
       youngest->gender = 'm';
       youngest->name = "Young";

/*
 * The five students, and their data, are initialized and added here.
 */
         
       student = kmalloc(sizeof(*student), GFP_KERNEL);
       student->month = 1;
       student->day = 1;
       student->year = 1991;
       student->gender = 'f';
       student->name = "Abigail";
       INIT_LIST_HEAD(&student->list);
       list_add_tail(&student->list, &birthday_list);

       student = kmalloc(sizeof(*student), GFP_KERNEL);
       student->month = 12;
       student->day = 31;
       student->year = 1994;
       student->gender = 'm';
       student->name = "John";
       list_add_tail(&student->list, &birthday_list);

       student = kmalloc(sizeof(*student), GFP_KERNEL);
       student->month = 6;
       student->day = 5;
       student->year = 1993;
       student->gender = 'm';
       student->name = "Oskar";
       list_add_tail(&student->list, &birthday_list);

       student = kmalloc(sizeof(*student), GFP_KERNEL);
       student->month = 12;
       student->day = 1;
       student->year = 1997;
       student->gender = 'm';
       student->name = "Walter";
       list_add_tail(&student->list, &birthday_list);

       student = kmalloc(sizeof(*student), GFP_KERNEL);
       student->month = 1;
       student->day = 2;
       student->year = 1998;
       student->gender = 'f';
       student->name = "Dea";
       list_add_tail(&student->list, &birthday_list);

/*
 * The original list of students is printed here.
 */

       list_for_each_entry(student, &birthday_list, list) {
         printk(KERN_INFO "Month: %d Day: %d Year: %d Gender: %c Name: %s\n", student->month, student->day, student->year, student->gender, student->name);
       }

/*
 * This block will iterate through the list to find the youngest student.
 * Per the prompt, the youngest student must be male.
 * Then, birthdays are compared.
 * If the current student is the youngest, their information is placed...
 * ... in the "youngest" object.
 */

       list_for_each_entry(student, &birthday_list, list) {

         if(student->gender == 'm') {
           if(student->year == youngest->year) {
             if(student->month == youngest->month) {
               if(student->day > youngest->day) {
                 youngest->month = student->month;
                 youngest->day = student->day;
                 youngest->year = student->year;
                 youngest->name = student->name;
               }
             }
             else if(student->month > youngest->month) {
               youngest->month = student->month;
               youngest->day = student->day;
               youngest->year = student->year;
               youngest->name = student->name;
             }
           }
           else if(student->year > youngest->year) {
             youngest->month = student->month;
             youngest->day = student->day;
             youngest->year = student->year;
             youngest->name = student->name;
           }
         }
       }

/*
 * This block deletes the youngest student from the list.
 * The youngest student is identified here through the name.
 * This is because the "youngest" object is not in the list itself.
 * However, the youngest student's name will match the name in the...
 * ..."youngest" object.
 */

       list_for_each_entry_safe(student, next, &birthday_list, list) {

         if(youngest->name == student->name) {
           printk(KERN_INFO "Removing %d %d %d %c %s\n", student->month, student->day, student->year, student->gender, student->name);
           list_del(&student->list);
         }

       }

/*
 *The new list is printed, with the youngest student removed.
 */

       list_for_each_entry(student, &birthday_list, list) {
         printk(KERN_INFO "Month: %d Day: %d Year: %d Gender: %c Name: %s\n", student->month, student->day, student->year, student->gender, student->name);
       }
       
       
       return 0;
}

void simple_exit(void) {
	
       printk(KERN_INFO "Removing Module\n");

       list_for_each_entry_safe(student, next, &birthday_list, list) {
          printk(KERN_INFO "Removing %d %d %d %c %s\n", student->month, student->day, student->year, student->gender, student->name);
          list_del(&student->list);
          kfree(student);
        }
}

module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel Data Structures");
MODULE_AUTHOR("SGG");
