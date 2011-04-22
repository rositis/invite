<?php

/**
 * User name of the inviter.
 */
class views_handler_field_invite_accept_link extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['reg_code'] = 'reg_code';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }
}
