<?php

/**
 * Invite class.
 *
 * A subclass of Entity is a requirement for creating a custom entity. It is
 * also a minimum requirement to define the methods included here, even as
 * empty functions.
 */
class Invite extends Entity {

  /**
   * Implements EntityInterface::id().
   */
  public function id() {
    return isset($this->id) ? $this->id : NULL;
  }

  /**
   * Implements EntityInterface::entityType().
   */
  public function entityType() {
    return 'invite';
  }

  /**
   * Implements EntityInterface::bundle().
   */
  public function bundle() {}

  /**
   * Implements EntityInterface::label().
   */
  public function label() {}

  /**
   * Implements EntityInterface::uri().
   */
  public function uri() {}

  /**
   * The user id of the profile owner.
   *
   * @var int
   */
  public $uid;

  /**
   *
   */
  public function type_details() {
    return invite_get_types($this->type);
  }

  /**
   * Returns the user owning this profile.
   */
  public function inviter() {
    return user_load($this->uid);
  }

  /**
   * Returns the user owning this profile.
   */
  public function invitee() {
    if (!empty($this->invitee)) {
      return user_load($this->invitee);
    }
    else {
      return FALSE;
    }
  }

  /**
   *
   */
  public function joinLink() {
    return url('invite/accept/' . $this->reg_code, array('absolute' => TRUE));
  }

  /**
   *
   */
  public function sendInvite() {
    $result = FALSE;
    if (!empty($this->sendNotification)) {
      // Notify other modules.
      entity_get_controller('invite')->invoke('send', $this);
      $result = TRUE;
    }
    else {
      $result = TRUE;
    }

    return $result;
  }

  /**
   * Sets a new user owning this profile.
   *
   * @param $account
   *   The user account object or the user account id (uid).
   */
  public function setUser($account) {
    $this->uid = is_object($account) ? $account->uid : $account;
  }

  /**
   *
   */
  public function __construct($values = array()) {
    if (isset($values['inviter'])) {
      $this->setUser($values['inviter']);
      unset($values['inviter']);
    }
    if (!isset($values['label']) && isset($values['type']) && $type = invite_get_types($values['type'])) {
      // Initialize the label with the type label, so newly created profiles
      // have that as interim label.
      $values['label'] = $type->label;
    }
    $this->sendNotification = TRUE;

    parent::__construct($values, 'invite'); //  TODO: Investigate.
  }

  /**
   *
   */
  public function status($readable = FALSE) {
    $options = invite_get_status_options();

    return $readable ? $options[$this->status] : $this->status;
  }

  /**
   *
   */
  protected function defaultLabel() {
    $entity = entity_metadata_wrapper('invite', $this);
    return t('Invitation #@number', array('@number' => $this->id()));
  }

  /**
   *
   */
  protected function defaultUri() {
    return array('path' => 'invite/' . $this->id());
  }

  /**
   *
   */
  public function save() {
    return parent::save();
  }

}
