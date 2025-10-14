<?php

// Should be set to 0 in production
error_reporting(E_ALL);

// Should be set to '0' in production
ini_set('display_errors', '1');

// Settings
$settings = [];

// Project root dir (1 parent)
$settings['root_dir'] = dirname(__DIR__, 1);


$settings['renderer'] = [
    // Template path
    'path' => $settings['root_dir'] . '/templates',
];

return $settings;

?>
