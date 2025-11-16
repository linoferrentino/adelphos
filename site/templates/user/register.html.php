<?php


 // Set layout
$this->setLayout('layout.html.php')
?>

<h3>Tell me about yourself and your family (help is available, click above) </h3>
<!-- <p> <x?= $bag['daemon_answer'] ?> </p> -->


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

		$(".form-input").css('background', 'white');

		pass1 = $('#password_1').val();
		pass2 = $('#password_2').val();

		if (pass1.length == 0)
			return;
		
		if (pass1 != pass2) {
			$('#password_2').css('backgroundColor', 'red');
			return;
		}

		$('#password_2').css('backgroundColor', 'white');
		submitButton.disabled = false;
		
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
