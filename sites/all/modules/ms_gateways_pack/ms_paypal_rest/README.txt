MoneyScripts PayPal REST API Payment Gateway
Author: Leighton Whiting
Copyright 2008-2013 MoneyScripts.net - Leighton Whiting

PayPal REST API integrates with the MS Core payment and checkout
system. This module will fully integrate PayPal to MS Core in such a way
that clients can make payments straight in the shop in PCI-compliant
way without leaving the actual shop page. It also supports advanced features
such as recurring payments and upgrading/downgrading recurring products.


INSTALLING MODULE
-------------------------------------

1. Download Libraries module from http://drupal.org/project/libraries

2. Enable MS PayPal Rest API and Libraries modules as usual: /admin/modules

3. Download PayPal Rest API SDK library from https://github.com/stripe/stripe-php and
   extract it to sites/all/libraries/rest-api-sdk-php so that the path to the autoload
   file is sites/all/libraries/rest-api-sdk-php/vendor/autoload.php

CONFIGURING PAYMENT METHOD
--------------------------

1. Create an account at https://developer.paypal.com/

2. Insert your Client ID and Secret Key at the configuration page
   admin/moneyscripts/gateways/ms_paypal_rest

3. Remember to test the functionality with the test keys before going live!


CREDITS
-------

MS PayPal REST API module was written by Leighton Whiting for MoneyScripts.net.
