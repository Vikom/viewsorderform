<?php

/**
 * Implements of hook_views_data()
 */
function viewsorderform_views_data() {

  $data['viewsorderform']['table']['group'] = t('Views order form');

  $data['viewsorderform']['table']['join'] = array(
    '#global' => array(),
  );

  $data['viewsorderform']['viewsorderform'] = array(
    'title' => t('Views order form'),
    'help' => t('Add a checkbox to order row'),
    'field' => array(
      'handler' => 'viewsorderform_handler_field_viewsorderform',
    ),
  );

  return $data;
}
