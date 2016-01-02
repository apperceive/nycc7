
replace into nyccftp.ms_cart_adjustments ( adjustment_id,cart_id,product_id,id,type,scope,display,value,weight,data)
select adjustment_id,cart_id,product_id,id,type,scope,display,value,weight,data from d6test.ms_cart_adjustments 
where adjustment_id = adjustment_id;

replace Into nyccftp.ms_cart_products ( cart_product_id,cart_id,type,id,name,module,qty,amount,recurring_schedule,changed,data)
select cart_product_id,cart_id,type,id,name,module,qty,amount,recurring_schedule,changed,data from d6test.ms_cart_products
where cart_id=cart_id;


replace Into nyccftp.ms_core_order_history ( id,oid,uid,hidden,status,message,created)
select id,oid,uid,hidden,status,message,created from d6test.ms_core_order_history
where id=id;

replace Into nyccftp.ms_core_payment_profiles ( id,uid,oid,remote_id,module,status,first_name,last_name,address,city,state,zip,country,phone,fax,email,cc_type,cc_num,exp_month,exp_year)
select id,uid,oid,remote_id,module,status,first_name,last_name,address,city,state,zip,country,phone,fax,email,cc_type,cc_num,exp_month,exp_year from d6test.ms_core_payment_profiles
where id=id;

replace Into nyccftp.ms_membership_plans ( mpid,name,description,main_amount,main_length,main_unit,trial_amount,trial_length,trial_unit,recurring,total_occurrences,signup_mail_subject,signup_mail_body,eot_mail_subject,eot_mail_body,cancel_mail_subject,cancel_mail_body,modify_mail_subject,modify_mail_body,expiring_mail_subject,expiring_mail_body,expiring_mail_days,start_grant_roles,start_remove_roles,expire_grant_roles,expire_remove_roles,weight,allow_roles,register_urls,expire_when,show_registration,show_account,modify_options,data)
select mpid,name,description,main_amount,main_length,main_unit,trial_amount,trial_length,trial_unit,recurring,total_occurrences,signup_mail_subject,signup_mail_body,eot_mail_subject,eot_mail_body,cancel_mail_subject,cancel_mail_body,modify_mail_subject,modify_mail_body,expiring_mail_subject,expiring_mail_body,expiring_mail_days,start_grant_roles,start_remove_roles,expire_grant_roles,expire_remove_roles,weight,allow_roles,register_urls,expire_when,show_registration,show_account,modify_options,data from d6test.ms_membership_plans
where mpid=mpid;

replace Into nyccftp.ms_memberships ( mid,oid,uid,mpid,status,expiration,start_date,current_payments,max_payments)
select mid,oid,uid,mpid,status,expiration,start_date,current_payments,max_payments from d6test.ms_memberships
where mid=mid;

replace Into nyccftp.ms_order_adjustments ( adjustment_id,oid,product_id,id,type,scope,display,value,weight,data)
select adjustment_id,oid,product_id,id,type,scope,display,value,weight,data from d6test.ms_order_adjustments
where adjustment_id=adjustment_id;

replace Into nyccftp.ms_order_products ( order_product_id,oid,type,id,name,module,qty,amount,recurring_schedule,data)
select order_product_id,oid,type,id,name,module,qty,amount,recurring_schedule,data from d6test.ms_order_products
where order_product_id=order_product_id;

replace Into nyccftp.ms_orders ( oid,uid,order_key,status,order_type,gateway,amount,total,currency,recurring_schedule,first_name,last_name,email_address,shipping_address,billing_address,data,created,modified,unique_key)
select oid,uid,order_key,status,order_type,gateway,amount,total,currency,recurring_schedule,first_name,last_name,email_address,shipping_address,billing_address,data,created,modified,unique_key from d6test.ms_orders
where oid=oid;

replace Into nyccftp.ms_payments ( pid,oid,type,transaction,recurring_id,gateway,amount,currency,data,created,modified)
select pid,oid,type,transaction,recurring_id,gateway,amount,currency,data,created,modified from d6test.ms_payments
where pid=pid;

replace Into nyccftp.ms_products_plans ( pid,bundle,sku,uid,name,cart_type,description,signup_mail_subject,signup_mail_body,eot_mail_subject,eot_mail_body,cancel_mail_subject,cancel_mail_body,modify_mail_subject,modify_mail_body,expiring_mail_subject,expiring_mail_body,expiring_mail_days,weight,recurring_schedule,modify_options,data)
select pid,module,sku,uid,name,type,description,signup_mail_subject,signup_mail_body,eot_mail_subject,eot_mail_body,cancel_mail_subject,cancel_mail_body,modify_mail_subject,modify_mail_body,expiring_mail_subject,expiring_mail_body,expiring_mail_days,weight,recurring_schedule,modify_options,data from d6test.ms_products_plans
where pid=pid;

replace Into nyccftp.ms_recurring_schedules ( id,oid,status,module,gateway,main_amount,main_length,main_unit,trial_amount,trial_length,trial_unit,total_occurrences,next_payment,current_payments,created,expiration,modified,failed_payments,notified)
select id,oid,status,module,gateway,main_amount,main_length,main_unit,trial_amount,trial_length,trial_unit,total_occurrences,next_payment,current_payments,created,expiration,modified,failed_payments,notified from d6test.ms_recurring_schedules
where id=id;