using UnityEngine;
using System.Collections.Generic;
using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.IO;

public class SimulationRecorder : MonoBehaviour
{

    private StreamWriter csvWriter;
    private string csvFilePath;
    private bool isRecording = false;
    private float startTime;
    private Rigidbody rb;


    private void Start()
    {
        rb = GetComponent<Rigidbody>();
        csvFilePath = Path.Combine(Application.dataPath, "ReplayData.csv");
        csvWriter = new StreamWriter(csvFilePath);
    }

    private void FixedUpdate()
    {
        if (Input.GetKeyDown(KeyCode.R))
        {
            if (!isRecording)
            {
                StartRecording();
            }
        }
        else if(Input.GetKeyDown(KeyCode.T))
        {
            if (isRecording)
            {
                StopRecording();
            }
        }

        if (isRecording)
        {
            Vector3 velocityVals = rb.velocity;

            Vector3 position = transform.localPosition;
            Quaternion rotation = transform.rotation;
            Vector3 angularVelocity = transform.InverseTransformDirection(rb.angularVelocity);
            Vector3 rotationSend = new Vector3(Mathf.Abs(360 - rotation.eulerAngles.x)%360,Mathf.Abs(360 - rotation.eulerAngles.y)%360,Mathf.Abs(360 - rotation.eulerAngles.z)%360);

            uint time = (uint)((Time.time)*1000000);
            string dataEntry = $"{position.x},{position.z},{position.y},{rotationSend.x},{rotationSend.z},{rotationSend.y},{-angularVelocity[0]},{-angularVelocity[2]},{-angularVelocity[1]},{velocityVals[0]},{velocityVals[2]},{velocityVals[1]},{time}";
            csvWriter.WriteLine(dataEntry);
        }
    }

    private void StartRecording()
    {
        Debug.Log("Recording started!");
        isRecording = true;
        startTime = Time.time;
    }

    private void StopRecording()
    {
        isRecording = false;
        string dataEntry = $"{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0}"; // Signals the end of the file has been reached.
        csvWriter.WriteLine(dataEntry);
        csvWriter.Close();
        Debug.Log("Recording saved to: " + csvFilePath);}




}
