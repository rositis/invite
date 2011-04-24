<?php

/**
 * Invite registration user name.
 */
class views_handler_field_invite_invitee_name extends views_handler_field_user_name {
  function construct() {
    parent::construct();
    $this->additional_fields['username'] = array(
      'table' => 'users',
      'field' => 'name',
    );
    $this->additional_fields['invitee'] = 'invitee';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
    $def = $this->definition;
    $def['table'] = 'users';
    $def['field'] = 'uid';
    $def['left_table'] = 'invite';
    $def['left_field'] = 'invitee';
    $def['type'] = 'INNER';
    $def['base'] = 'invitee';
    if (!empty($def['join_handler']) && class_exists($def['join_handler'])) {
      $join = new $def['join_handler'];
    }
    else {
      $join = new views_join();
    }
    $join->definition = $def;
    $join->construct();
    $join->adjusted = TRUE;
    $alias = 'users';
  }

  function render_link($data, $values) {
    if (!empty($this->options['link_to_user']) && user_access('access user profiles') && $values->{$this->aliases['invitee']}) {
      $data = l($values->{$this->aliases['username']}, 'user/' . $values->{$this->aliases['invitee']});
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
