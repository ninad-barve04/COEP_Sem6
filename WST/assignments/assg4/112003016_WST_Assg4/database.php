<?php

    $hostname="localhost";
    $user="__username__";
    $password="__database_password__";
    $database="__databasename_";


    $conn = new mysqli( $hostname, $user, $password, $database);

    if( $conn->connect_error){
        die ("connection failed " . $conn->connect_error);
    }
  

    function get_contacts($connection){
                
        $query = "SELECT * FROM contacts";

        $results = $connection->query( $query);
        if( $results == true)
        {
            if( $results->num_rows > 0){
                $data = mysqli_fetch_all( $results,MYSQLI_ASSOC);
            }
            else{
                $data = "No Records Found";
            }
        }
        else{
            $data = mysqli_error($connection);
        }

        return $data;
    }

    function get_contact_for_username($connection, $username ){
        $query = "SELECT username, email FROM contacts where username like '".$username."'";

        $results = $connection->query( $query);
        if( $results == true)
        {
            if( $results->num_rows > 0){
                $data = mysqli_fetch_all( $results,MYSQLI_ASSOC);
            }
            else{
                $data = "No Records Found";
            }
        }
        else{
            $data = mysqli_error($connection);
        }

        return $data;
    }

    function get_contact_for_email( $connection,  $email){
        $query = "SELECT username, email FROM contacts where email like '".$email."' ";
 
        $results = $connection->query( $query);
        if( $results == true)
        {
            if( $results->num_rows > 0){
                $data = mysqli_fetch_all( $results,MYSQLI_ASSOC);
            }
            else{
                $data = "No Records Found";
            }
        }
        else{
            $data = mysqli_error($connection);
        }

        return $data;
    }

    function add_contact($connection,$data){

        $query = "INSERT INTO contacts ( firstname, lastname, username, email, state, country) 
                    VALUES ('".$data['firstname']."','".$data['lastname']."','".$data['username']."','".$data['email']."','".$data['state']."','".$data['country']."')  ";

   
        $results = $connection->query( $query);
        if( $results == true){
            $msg = "Contact added successfully";
        }
        else{
            $msg = mysqli_error($connection);
        }
        return $msg;
    }
?>