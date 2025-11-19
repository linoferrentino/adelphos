<?php


 // Set layout
$this->setLayout('layout.html.php')
?>

<h3>Tell me about yourself and your family. </h3>
<!-- <p> <x?= $bag['daemon_answer'] ?> </p> -->

<h4>Please use at least 5 characters for user/family and password</h4>


    <form action="/user/do_register"
          id="reg_form" class="form-container" method="post" autocomplete="off">

<div class="form-row">
            <label class="form-label" for="userid">User name:</label>
            <input class="form-input" name="userid" type="text" id="userid">

</div>
<div class="form-row">
            <label class="form-label" for"family">Family name:</label>
            <input class="form-input"  id="family" type="text" name="family">
</div>
<div class="form-row">
            <label class="form-label" for"email">Email:</label>
            <input class="form-input"  id="email" type="text" name="email">
</div>
<div class="form-row">
            <label class="form-label" for"password">Password:</label>
            <input class="form-input"  id="password_1" type="password" name="password">
</div>

<div class="form-row">
            <label class="form-label" for"password">Confirm password:</label>
            <input class="form-input"  id="password_2" type="password" name="password_2">
</div>
<div class="form-row">
            <button type="submit" class='form-submit' disabled id="register_submit">Register</button>
</div>
    </form>

<script>


document.addEventListener('DOMContentLoaded', function() {
	
	var form = document.getElementById('reg_form');
	var submitButton = document.getElementById('register_submit');
	form.addEventListener('input', function() {


		//something on user and password
		user = $('#userid').val();
		family = $('#family').val();
		can_submit = true;

		if (user.length < 5) {
			$('#userid').css('backgroundColor', 'red');
			can_submit = false;
		} else {
			$('#userid').css('backgroundColor', 'white');
		}

		if (family.length < 5) {
			$('#family').css('backgroundColor', 'red');
			can_submit = false;
		} else {
			$('#family').css('backgroundColor', 'white');
		}


		pass1 = $('#password_1').val();
		pass2 = $('#password_2').val();

		if (pass1.length < 5) {
			$('#password_1').css('backgroundColor', 'red');
			can_submit = false;
		} else {
			$('#password_1').css('backgroundColor', 'white');
		}
		
		if (pass1 != pass2) {
			$('#password_2').css('backgroundColor', 'red');
			can_submit = false;
		} else {
			$('#password_2').css('backgroundColor', 'white');
		}


		if (can_submit) {
			submitButton.disabled = false;
		}
		
	});
});



$(document).ready(function () {
   $('#reg_form').submit(function (e) {
       e.preventDefault();

       $.ajax({
           url: '/user/do_register',
           type: 'POST',
	   data: { email: $('#email').val(), userid: $("#userid").val(), 
	   family: $("#family").val(), password: $("#password_1").val() }, 
           success: function (response) {
		   if (response.redirectUrl){
			   window.location.href = response.redirectUrl;
		   }
               //console.log(response.message); // Handle success
               //alert("Success:" +  response.message);
           },

	   error: function(xhr, status, error) {
		   console.log(xhr.responseText)
		   var errorMessage = xhr.responseText ? JSON.parse(xhr.responseText).data['errors']['email'][0] : "An error occurred";
		   document.getElementById('email').style.backgroundColor = 'red';
		   alert("Please correct the errors " + errorMessage);
	   }
           
       });
   });
});


</script>
