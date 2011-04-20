<?php

/**
 * User name of the inviter.
 */
class views_handler_field_invite_uid_name extends views_handler_field_user_name {
  function construct() {
    parent::construct();
    $this->additional_fields['username'] = array(
      'table' => 'users',
      'field' => 'name',
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render_link($data, $values) {
    if (!empty($this->options['link_to_user']) && user_access('access user profiles') && $values->{$this->aliases['uid']}) {
      $data = l($values->{$this->aliases['username']}, 'user/' . $values->{$this->aliases['uid']});
    }
    else {
      $data = $values->{$this->aliases['username']};
    }
    return $data;
  }

  function render($values) {
    return $this->render_link(check_plain($values->{$this->field_alias}), $values);
  }
}
