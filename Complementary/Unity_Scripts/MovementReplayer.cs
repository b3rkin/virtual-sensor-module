using System.Collections.Generic;
using UnityEngine;
using System.IO;
using System;

public class MovementReplayer : MonoBehaviour
{
    public string csvFilePath = "Assets/Resources/ReplayData.csv";  // Path to the CSV file
    public GameObject targetObject;  // The game object to adjust
    private StreamReader reader;
    private bool isReplaying = false;
    private float replayStartTime;

    private void Start()
    {

    }

    void FixedUpdate()
    {

        if (isReplaying && !reader.EndOfStream)
        {
            ReplayData();
        }
        if(isReplaying && reader.EndOfStream){
            Debug.Log("Replaying stopped");
            reader.Close();
            reader = null;
            isReplaying = false;
        }
    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Y) && !isReplaying)
        {
            StartReplaying();
        }
        else if (Input.GetKeyDown(KeyCode.Y) && isReplaying){
            Debug.Log("Replaying stopped");
            reader.Close();
            reader = null;
            isReplaying = false;
        }
    }

    private void StartReplaying()
    {
        Debug.Log("Replaying started!");
        if (!string.IsNullOrEmpty(csvFilePath))
        {
            reader = new StreamReader(csvFilePath);
        }
        isReplaying = true;
    }

    private void ReplayData()
    {
        try{
            string line = reader.ReadLine();
            string[] values = line.Split(',');

            float posX  = float.Parse(values[0]);
            float posY  = float.Parse(values[1]);
            float posZ  = float.Parse(values[2]);
            float rotX = float.Parse(values[3]);
            float rotY = float.Parse(values[4]);
            float rotZ = float.Parse(values[5]);

            // Adjust the targetObject's parameters
            targetObject.transform.localPosition = new Vector3(posX, posZ, posY);
            targetObject.transform.localEulerAngles = new Vector3(360-rotX, 360-rotZ, 360-rotY);
        }
        catch (Exception e)
        {
            Debug.Log(e.Message+"\n");
            Debug.Log("Replaying stopped");
            reader.Close();
            reader = null;
            
            isReplaying = false;
        }
            
    }



    
}