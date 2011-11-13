<?php

/**
 * @file
 * Hooks provided by Invite module.
 */

/**
 * Respond to an invitation being sent out.
 *
 * This hook is invoked right after an invitation has been created and sent out.
 *
 * @param $invite
 *   The invite object that has just been created.
 */
function hook_invite_send($invite) {
}

/**
 * Respond to an invitation withdrawal.
 *
 * This hook is invoked right after an invitation has been withdrawn.
 *
 * @param $invite
 *   The invite object that has just been withdrawn.
 */
function hook_invite_withdraw($invite) {
}

/**
 * Respond to an invitation acceptance.
 *
 * This hook is invoked right after a user account has been created by using an
 * invitation.
 *
 * @param $invite
 *   The invite object that has just been accepted.
 * @param $accout
 *   The newly created user account.
 */
function hook_invite_accept($invite, $accout) {
}

/**
 * Alter roles that are being added to the user who has just accepted an invitation
 * and created an account.
 *
 * This hook is invoked after the target roles for the new registrant have been
 * determined, but before they have been assigned to the user. Modify $roles to
 * change what roles will be added to the user.
 *
 * @param $roles
 *   An array of role IDs that will be added to the user.
 * @param $invite
 *   The invite that the user has accepted.
 * @param $account
 *   The new account that is being created. This object comes from hook_user_presave,
 *   and is not a fully populated user object.
 */
function hook_invite_target_roles(&$roles, $invite, $account) {
}

