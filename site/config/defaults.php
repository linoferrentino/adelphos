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

$settings['logger'] = [
    // Log file location in the logs folder in the project root
    'path' => dirname(__DIR__) . '/logs',
    // Default log level
    'level' => \Monolog\Level::Debug,
];


$settings['session'] = [
    'name' => 'session-name',
    // 5h session lifetime
    'lifetime' => 18000, // Time in seconds
];

// adelphos backend
$settings['adelphos-backend'] = [
	'backend-instance' => 'sock'
];


$settings['error'] = [
    // Must be set to false in production
    'display_error_details' => true,
    // Whether to log errors or not
    'log_errors' => false,
];


return $settings;

?>
