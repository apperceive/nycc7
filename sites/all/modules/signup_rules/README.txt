------------------------------------------------------------------
|||||||||||||||||||||||||| SIGNUP RULES ||||||||||||||||||||||||||
------------------------------------------------------------------

 Maintained by Jordan Halterman <jordan.halterman@gmail.com>
 http://drupal.org/project/signup_rules

 Requirements:
 -------------
 1. Signup module (http://drupal.org/project/signup)
 2. Rules module (http://drupal.org/project/rules)


 Installation:
 -------------
 1. Download and install both the Signup module and Rules module.
 2. To configure new events/conditions/actions navigate to
    'administer >> rules >> triggered rules'.


 Provided events:
 -----------------
 1.  User signed up for content
   - This event is triggered when a user first signs up for content.
 2.  User signed up from content signup form
   - Triggered when a user signs up from the signup form on content.
 3.  User signed up from registration form
   - Triggered when a user signs up from the user registration form.
 4.  Signup was changed
   - This event is triggered when the user has updated a signup.
 5.  Signup was cancelled
   - This event is triggered when an individual user\'s signup has been cancelled.
 6.  Signup was marked attended
   - Triggered when a user's signup has been marked attended.
 7.  Signup was marked not attended
   - Triggered when a user's signup has been marked not attended.
 8.  Signups were opened for content
   - This event is triggered when the status of signups for content is changed to open.
 9.  Signups were closed for content
   - This event is triggered when the status of signups for content is changed to closed.
 10. Signup form is being displayed


 Provided conditions:
 --------------------
 1.  Signup total is equal to/great to/less than
   - This condition determines whether the signup total of the content is less than,
     equal to, or greater than a given numeric value.
 2.  User is signed up for content
   - This condition determines whether a user is signed up for a piece of content.
 3.  Signups are open for content
   - This condition determines whether signups are open for the content.
 4.  Signup is marked attended
   - This condition determines whether a signup has been marked 'attended'.


 Provided actions:
 -----------------
 1.  Load signup data
   - This action allows individual signup data to be loaded from a node and user.
 2.  Sign up user to content
   - Signs up a user to content
 3.  Cancel signup
   - Cancels an individual signup.
 4.  Cancel all signups for content
   - This action cancels all signups for a given piece of content.
 5.  Cancel all signups for user
   - This action cancels all signups for a given user.
 6.  Open signups for content
   - This action opens up signups for a piece of content.
 7.  Close signups for content
   - This action opens up signups for a piece of content.
 8.  Mark signup as attended
   - This action marks a signup as attended.
 9.  Mark signup as not attended
   - This action marks a signup as not attended.
 10. Send mail to all users signed up for content (by scottrigby)
   - Sends an e-mail to attended, not attended, or all users signup up for content.
 11. Prevent display of signup form
   - Prevents the display of the signup form.


 To make additional feature requests please visit the project page at
 http://drupal.org/project/signup_rules.


 Thanks to meichr, Magnus, and MasterChief for working on the initial
 development of this project.


 Maintainer/project lead: jordojuice
