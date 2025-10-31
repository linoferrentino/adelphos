
<?php


/*
 * The menu items are dependent from the state of the application
 *
 */
function app_add_menu_items()
{
}

function make_bag_parameters($parameters, $session = null) 
{

	if ( \AD_DEBUG == 'y' ) {

		$bag_params = [
			'_bag_debug_value' => \AD_DEBUG,
			'_bag_date' => date("Y-m-d H:i:s"),
			'_bag_session' => json_encode($session)
		];

	} else {

		$bag_params = [];

	}


	$bag_params = array_merge($parameters, $bag_params);

	$bag = [

		"bag" => $bag_params

	];


	return $bag;

}


function html(?string $text = null): string
{
    return htmlspecialchars($text ?? '', ENT_QUOTES | ENT_SUBSTITUTE, 'UTF-8');
}


?>
