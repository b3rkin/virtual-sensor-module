using UnityEngine;

public class KeyboardController : MonoBehaviour
{
    public float speed = 0.3f; // The fixed speed at which the object moves
    private Rigidbody rb;

    private void Start()
    {
        rb = GetComponent<Rigidbody>();
    }

    private void FixedUpdate()
    {
        float moveX = Input.GetAxis("Horizontal");
        float moveY = Input.GetAxis("Vertical");

        float moveZ = 0f;
        if (Input.GetKey(KeyCode.X))
        {
            moveZ = speed;
        }
        else if (Input.GetKey(KeyCode.Z))
        {
            moveZ = -1*speed;
        }

        Vector3 movement = new Vector3(moveX, moveZ, moveY);

        rb.velocity = movement * speed;
    }
}