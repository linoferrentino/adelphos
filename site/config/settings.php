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

return $settings;

?>
