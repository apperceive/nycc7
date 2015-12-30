<?php

class NYCCUserMigration extends DrupalUser6Migration {
  public function __construct($variables = array()) {
    parent::__construct($variables);
    $this->description = t('Migrate NYCC users');

    $source_fields = array(
      'uid' => t('User ID'),
      'roles' => t('The set of roles assigned to a user.'),
    );

    $query = db_select(NYCC_MIGRATION_DATABASE_NAME .'.users', 'u')
      ->fields('u', array('uid', 'name', 'pass', 'mail', 'created', 'access', 'login', 'status', 'init'))
      //->condition('u.name', array('admin', 'rgates'), 'NOT IN')
      ->condition('u.uid', 0, '>')
      ->condition('u.status', 0, '>');
      // TODO: how add an 'or' to filter out old inactives
      // ->condition('u.updated', time-3*365*24*60*60, '>');     // three years inactive
      
    $this->source = new MigrateSourceSQL($query, $source_fields);
    //$this->destination = new MigrateDestinationUser();
    // preserve D6 passwords
    // so says https://www.drupal.org/node/1349758
    $this->destination = new MigrateDestinationUser(array('md5_passwords' => TRUE));

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'uid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'D6 Unique User ID',
          'alias' => 'u',
        )
      ),
      MigrateDestinationUser::getKeySchema()
    );

    
    
    // reuse old id's. omit creating new users
    $this->addFieldMapping('is_new')->defaultValue(TRUE);
    $this->addFieldMapping('uid', 'uid');
    
    // Make the mappings
    $this->addFieldMapping('name', 'name')->dedupe('users', 'name');
    $this->addFieldMapping('pass', 'pass');
    $this->addFieldMapping('mail', 'mail')->dedupe('users', 'mail');
    $this->addFieldMapping('language')->defaultValue('');
    $this->addFieldMapping('theme')->defaultValue('');
    $this->addFieldMapping('signature')->defaultValue('');
    $this->addFieldMapping('signature_format')->defaultValue('filtered_html');
    $this->addFieldMapping('created', 'created');
    $this->addFieldMapping('access', 'access');
    $this->addFieldMapping('login', 'login');
    $this->addFieldMapping('status', 'status');
    $this->addFieldMapping('picture')->defaultValue(0);
    $this->addFieldMapping('init', 'init');
    $this->addFieldMapping('timezone')->defaultValue(NULL);
    $this->addFieldMapping('path')->issueGroup(t('DNM'));          // DO NOT MIGRATE
    //$this->addFieldMapping('pathauto_perform_alias')->defaultValue('1');
    $this->addFieldMapping('roles', 'roles');
  }
  
  public function prepareRow($current_row) {
    $source_id = $current_row->uid;
    $query = db_select(NYCC_MIGRATION_DATABASE_NAME .'.users_roles', 'r')
      ->fields('r', array('uid', 'rid'))
      ->condition('r.uid', $source_id, '=');
    $results = $query->execute();
    $roles = array(DRUPAL_AUTHENTICATED_RID => DRUPAL_AUTHENTICATED_RID);
    foreach ($results as $row) {
      $roles[$row->rid] = $row->rid;
    }
    $current_row->roles = $roles;
    return TRUE;
    // return FALSE if you wish to skip a particular row
  }
}



class NYCCUserPictureMigration extends DrupalUser6Migration {
  // ensure images attached to users?

}




class NYCCUserProfileMigration extends DrupalNode6Migration {


}




class NYCCUserPagesMigration extends DrupalNode6Migration {

  public function __construct($variables = array()) {
    parent::__construct($variables);
    $this->description = t('Migrate NYCC page nodes');

    $query = db_select(NYCC_MIGRATION_DATABASE_NAME .'.users', 'u')
      ->fields('u', array('uid', 'name', 'pass', 'mail', 'created', 'access', 'login', 'status', 'init'))
      //->condition('u.name', array('admin', 'rgates'), 'NOT IN')
      ->condition('u.uid', 0, '>')
      ->condition('u.status', 0, '>');
      // TODO: how add an 'or' to filter out old inactives
      // ->condition('u.updated', time-3*365*24*60*60, '>');     // three years inactive
      
    $this->source = new MigrateSourceSQL($query, $source_fields);
    
    $this->destination = new MigrateDestinationNode();


    
    
    // reuse old id's. omit creating new users
    $this->addFieldMapping('is_new')->defaultValue(TRUE);
    $this->addFieldMapping('nid', 'nid');
    
  }
  
  public function prepareRow($current_row) {
    return parent::prepareRow($current_row);
  }

}


// TODO: move this to node.inc

class NYCCPageNodeMigrator extends DrupalNode6Migration {

  /**
   * Constructor method
   */
  public function __construct(array $arguments) {

    // Call parent method
    parent::__construct($arguments);

    // Extra Fields
    $this->addFieldMapping('field_post_image', 'field_post_image')->sourceMigration('FileMigration');
    $this->addFieldMapping('field_post_image:file_class')->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_other_images', 'field_other_images')->sourceMigration('FileMigration');
    $this->addFieldMapping('field_other_images:file_class')->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_post_files', 'field_post_files')->sourceMigration('FileMigration');
    $this->addFieldMapping('field_post_files:file_class')->defaultValue('MigrateFileFid');

  }

  /**
   * complete() method from Migrate module
   * Runs after node_save operation
   */
  public function complete($entity, stdClass $source_row) {

    // Due to problems in translation migration, we are using a different database for its migration.
    write_tnid_to_db($entity, $source_row);

  }

}