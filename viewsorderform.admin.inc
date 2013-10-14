<?php

/**
 * @file
 * Administration page callbacks for Views order form.
 */

/**
 * Implements hook_admin_settings().
 */
function viewsorderform_admin_settings_form($form_state) {

  $form['viewsorderform_mail_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mail settings'),
    '#description' => t('Settings for the order email.'),
  );

  $form['viewsorderform_mail_settings']['viewsorderform_mail'] = array(
    '#title' => t('Email address'),
    '#type' => 'textfield',
    '#default_value' => variable_get('viewsorderform_mail'),
    '#size' => 120,
    '#maxlength' => 120,
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
