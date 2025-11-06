<?php

namespace App\Backend\mock;

use App\Backend\AdelphosBE;
use App\Data\CurrencyCollection;

/*
 * MockAdelphosBe the mock backend used to test the application.
 *
 *
 */

class MockAdelphosBE implements AdelphosBE {

	public function echo_me(string $text) : string {

		return "echo " . $text;

	}

	// the mock adelphos must return a cursor to the "fake" table of the
	// currencies.
	public function get_currencies() : CurrencyCollection {

		return new CurrencyCollection();
	}
}


?>
