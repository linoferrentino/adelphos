
<?php


// I instantiate a backend based on a connection string.
function  instantiate_back_end(string $backend_conn) : \App\Backend\AdelphosBE {


	// for now we only support the mock backend
	if ($backend_conn == 'mock') {
		return new \App\Backend\mock\MockAdelphosBE();
	}

	throw new Exception("Unsupported backend " . $backend_conn);

}


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
			//'_bag_session' => print_r($session, true)
			'_bag_session_ob' => $session
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
