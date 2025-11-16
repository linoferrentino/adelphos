<?php

namespace App\Backend;

use App\Data\CurrencyCollection;
use App\Data\UserData;
use App\Data\UserRegistrationData;


/*
 * Adelphos Backend, the abstract interface to the adelphos backend
 * which the web site will use.
 *
 * The functions here do not return an error, they throw an exception which
 * will be handled above.
 *
 * Copyright (c) 2025 Lino Ferrentino, lino.ferrentino@gmail.com
 *
 * GPL3 or above.
 *
 *
 *
 *
 */

interface AdelphosBE 
{

	// just a test
	public function echo_me(string $text) : string;


	// we get the currency backend


	public function get_currencies() : CurrencyCollection;


	// we can register a new user, with a user password
	public function add_user(UserRegistrationData $user_data);

}






?>
