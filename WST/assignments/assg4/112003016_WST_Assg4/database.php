<?php


  

    function get_contacts(){
        $hostname="localhost";
        $user="wstninad";
        $password="iamninad";
        $database="wst_assignment_4";
    
    
        $conn = new mysqli( $hostname, $user, $password, $database);
    
        if( $conn->connect_error){
            die ("connection failed " . $conn->connect_error);
        }

        
        $query = "SELECT * FROM contacts";

        $results = $conn->query( $query);
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
            $data = mysqli_error($conn);
        }

        return $data;
    }
?>