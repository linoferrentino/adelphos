
<?php

define('SES_REGISTRATION_KEY', 'session-registration-key');

function html(?string $text = null): string
{
    return htmlspecialchars($text ?? '', ENT_QUOTES | ENT_SUBSTITUTE, 'UTF-8');
}


?>
