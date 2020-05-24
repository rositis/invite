<?php


class InviteType extends Entity {
  public $type;
  public $label;
  public $weight = 0;

  /**
   * @inheritDoc
   */
  public function id() {
    return isset($this->id) ? $this->id : NULL;
  }

  /**
   * @inheritDoc
   */
  public function entityType() {
    return 'invite_type';
  }

  /**
   * @inheritDoc
   */
  public function label() {}

  /**
   * @inheritDoc
   */
  public function uri() {}

  /**
   * Array with the argument keys.
   *
   * This is used to allow creation of entity metadata properties based
   * on the argument keys.
   *
   * @see InviteMetadataController::entityPropertyInfo()
   *
   * @var array
   */
  public $argument_keys = array();

  public $argument = array();

  public $invite_sending_controller = array();

  /**
   * InviteType constructor.
   *
   * @param array $values
   */
  public function __construct($values = array()) {
    parent::__construct($values); // removed second parameter 'invite_type'.
    if (empty($this->invite_sending_controller)) {
      $controllers = db_select('invite_sending_controller', 'isc')
        ->fields('isc')
        ->condition('type', array($this->type))
        ->execute();
      $this->invite_sending_controller = array();
      foreach ($controllers as $controller) {
        $this->invite_sending_controller[$controller->name] = $controller->name;
      }
    }
  }

  /**
   * @param $controller_name
   *
   * @return bool
   */
  public function hasSendingController($controller_name) {
    $result = FALSE;
    if (!empty($this->invite_sending_controller[$controller_name])) {
      $result = TRUE;
    }
    return $result;
  }

  /**
   * @return bool
   */
  public function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }

  /**
   * @return int|void
   */
  public function save() {
    parent::save();
    $controllers = array();
    // Remove old controller entries.
    db_delete('invite_sending_controller')
      ->condition('type', $this->type)
      ->execute();
    // Add controllers.
    foreach ($this->invite_sending_controller as $controller_name) {
      $module = !empty($this->module) ? $this->module : '';
      if (!in_array($controller_name, $controllers)) {
        $controllers[] = $controller_name;
        $record = db_insert('invite_sending_controller')
          ->fields(array(
            'type' => $this->type,
            'module' => $module,
            'name' => $controller_name,
          ));
        try {
          $record->execute();
        }
        catch(Exception $e) {
          watchdog('invite', $e->getMessage());
        }
      }
    }
  }

  public function delete() {
    parent::delete();
    db_delete('invite_sending_controller')
      ->condition('type', $this->type)
      ->execute();
  }
}
