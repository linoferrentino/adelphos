<?php

namespace App\Backend;

use App\Data\CurrencyCollection;
use App\Data\UserData;


/*
 * Adelphos Backend, the abstract interface to the adelphos backend
 * which the web site will use.
 *
 * Copyright (c) 2025 Lino Ferrentino, lino.ferrentino@gmail.com
 *
 * GPL3 or above.
 */

interface AdelphosBE 
{

	// just a test
	public function echo_me(string $text) : string;


	// we get the currency backend


	public function get_currencies() : CurrencyCollection;


	// we can register a new user, with a user password

	public function add_user(UserData $user);

}






?>
