-- SUMMARY --

This module augments Drupal Core's Statistics module by collecting additional
information. It does so with no greater performance overhead than Core
Statistics itself introduces.

This module won't do you much good unless you have the Statistics module enabled
and "Enable access log" checked at admin/reports/settings.


-- FEATURES --

* Adds cache status and user-agent fields to the access log.
* Additional fields are integrated with views (filtering, sorting, etc).


-- WHY INSTALL THIS MODULE --

Install this module if you need to have the ability to answer questions like:

* What is my website's cache hit rate throughout the day, week, month, etc?
* How does page generation time of cached pages compare to uncached pages?
* When does Google crawl my website and what kind of page generation time does
  it see?


-- INSTALLATION --

* Ensure that the core Statistics module is already enabled.
* Install and enable this module.
* Ensure that "Enable access log" is checked at admin/reports/settings.
