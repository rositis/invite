<?php


class InviteTypeController extends EntityPlusController {

  /**
   * @param array $values
   *
   * @return mixed|object
   */
  public function create(array $values = array()) {
    $values += array(
      'label' => '',
      'description' => '',
      'data' => array(),
    );
    return parent::create($values);
  }

  /**
   * Save Invite Type.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    parent::save($entity, $transaction);
    /*
     * Rebuild menu registry. We do not call menu_rebuild directly, but set
     * variable that indicates rebuild in the end.
     * @see _http://drupal.org/node/1399618
     */
    state_set('menu_rebuild_needed', TRUE);
  }
}
