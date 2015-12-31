<?php

function nycc_og_admin() {
  $form = array();
	$role_names = og_roles('node', 'Group');


	$plans = ms_products_get_plans('ms_membership');
	if((! empty($plans))) {
		$plan_names = array();
		foreach($plans as $plan) {
			$plan_names[$plan->pid] = $plan->name;
		}
		$form['nycc_moneysuite_membership_plans'] = array(
			'#type' => 'checkboxes',
			'#title' => ' Active Membership Plans',
			'#options' => $plan_names,
			'#default_value' => variable_get('nycc_moneysuite_membership_plans'),
			'#description' => t('Active Membership Plans indicating a user is a paid member.')
		);		
	} else {
		drupal_set_message("No active Membership Plans found");		
		
	}
	

  return system_settings_form($form);
}



