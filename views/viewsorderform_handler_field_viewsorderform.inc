<?php

/**
 * @file
 * Views handlers for Views order form.
 */

/**
 * A handler to provide a field that is completely custom by the administrator.
 *
 * @ingroup views_field_handlers
 */
class viewsorderform_handler_field_viewsorderform extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = 'nid';
    $this->additional_fields['title'] = 'title';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    // Render a Views form item placeholder.
    return '<!--form-item-' . $this->options['id'] . '--' . $this->view->row_index . '-->';
  }

  /**
   * Add to and alter the form created by Views.
   */
  function views_form(&$form, &$form_state) {
    // Display stage 2 if $form_state['storage']['confirm'] is set
    if (isset($form_state['storage']['confirm'])) {
      // We don't want to display all items from view.
      // @TODO A cleaner solution should exist.
      unset($form['output']['#markup']);
      
      $form['full_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Full name'),
        '#required' => TRUE,
      );
      
      $form['telephone'] = array(
        '#type' => 'textfield',
        '#title' => t('Telephone'),
        '#required' => TRUE,
      );

      $form['email'] = array(
        '#type' => 'textfield',
        '#title' => t('Email'),
        '#required' => TRUE,
      );

      $form['street'] = array(
        '#type' => 'textfield',
        '#title' => t('Street'),
        '#required' => TRUE,
      );

      $form['zip_code'] = array(
        '#type' => 'textfield',
        '#title' => t('Zip code'),
        '#required' => TRUE,
      );

      $form['city'] = array(
        '#type' => 'textfield',
        '#title' => t('City'),
        '#required' => TRUE,
      );

      $form['country'] = array(
        '#type' => 'textfield',
        '#title' => t('Country'),
        '#required' => TRUE,
      );

      $form['message'] = array(
        '#type' => 'textarea',
        '#title' => t('Message to Nordic Biosite'),
        '#required' => FALSE,
      );

      // The ordered items.
      $order = viewsorderform_order($form_state);
      $form['#suffix'] = $order;

      $form['#action'] = '/literature';
    }

    else {
      // Create a container for our replacements
      $form[$this->options['id']] = array(
        '#type' => 'container',
        '#tree' => TRUE,
      );
      // Iterate over the result and add our replacement fields to the form.
      foreach ($this->view->result as $row_index => $row) {
        // Add order checkbox to the form.
        $form[$this->options['id']][$row_index] = array(
          '#type' => 'checkbox',
          '#title' => t('Add to order'),
        );
      }
      if (empty($form_state['storage']['item'])) {
        $form['request_order'] = array(
          '#type' => 'submit',
          '#value' => 'Request order',
        );
      }
      $form['#action'] = '/literature';
    }
  }

  /**
   * Form validate method.
   */
  function views_form_validate($form, &$form_state) {
    if ($form_state['clicked_button']['#id'] == 'edit-request-order') {
    }
    else {
      if (!valid_email_address($form_state['values']['email'])) {
        form_set_error('email', t('You must enter a valid e-mail address.'));
      }
    }
  }

  /**
   * Form submit method.
   */
  function views_form_submit($form, &$form_state) {
    if ($form_state['clicked_button']['#id'] == 'edit-request-order') {
      $form_state['storage']['confirm'] = TRUE;
      $form_state['storage']['items'] = $form_state['values'];
      $form_state['rebuild'] = TRUE;
    }
    else {
      $order = array();

      // Order values from first step.
      foreach ($form_state['storage']['items']['viewsorderform'] as $key => $value) {
        if ($value === 1) {
          $order[] = $form_state['build_info']['args'][0]->result[$key]->node_title;
        }
      }

      // Send order by mail.
      if (!empty($order)) {
        $to = 'viktor.miranda@symbio.com';
        $params = array(
          'order' => $order,
          'full_name' => $form_state['values']['full_name'],
          'telephone' => $form_state['values']['telephone'],
          'email' => $form_state['values']['email'],
          'street' => $form_state['values']['street'],
          'zip_code' => $form_state['values']['zip_code'],
          'city' => $form_state['values']['city'],
          'country' => $form_state['values']['country'],
          'message' => $form_state['values']['message'],
        );
        $from = variable_get('site_mail', ini_get('sendmail_from'));

        drupal_mail('viewsorderform', 'viewsorderform_order', $to, language_default(), $params, $from);

        // Redirect to confirmation page.
        drupal_goto('literature/confirmation');
      }
    }
  }
}

function viewsorderform_order($form_state) {
  $output = '<aside class="order-block"><h2>' . t('Your order') . '</h2>';
  
  $output .= '<ul>';
  foreach ($form_state['values']['viewsorderform'] as $key => $value) {
    if ($value === 1) {
      $image = $form_state['build_info']['args'][0]->result[$key]->field_field_literature_image[0]['rendered'];
      $output .= '<li>';
      $output .= drupal_render($image);
      $output .= $form_state['build_info']['args'][0]->result[$key]->node_title . '</li>';
    }
  }
  $output .= '</ul></aside>';

  return $output;
}
