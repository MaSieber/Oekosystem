using UnityEngine;
using System.Collections;

[RequireComponent(typeof(PlayerPhysics))]
public class Player : MonoBehaviour {

    public float PlayerVelocity = 1.0f;

    //[HideInInspector]
    public PlayerPhysics playerPhysics;
    private bool bCanJump = false;
    private bool bJumping = false;
    private bool bFalling = false;
    private int iJumpCounter = 0;

    public float jumpHeight = 8.0f;
    public float jumpTime = 2.0f;

    protected Vector2 targetSpeed = Vector2.zero;
    protected Vector2 currentSpeed = Vector2.zero;
    public Vector2 acceleration = Vector2.zero;

    private Vector3 lastDirection = Vector3.zero;
    private Vector2 movementDirection;

	// Use this for initialization
	void Start () {
	
	}

    protected float IncrementTowards(float n, float target, float a)
    {
        if (n == target)
            return n;
        else
        {
            float dir = Mathf.Sign(target - n);
            if (transform.eulerAngles.y >= 180.0f)
                dir = -1;
            n += a * Time.deltaTime * dir;
            return (dir == Mathf.Sign(target - n)) ? n : target;
        }
    }

    void handleJump()
    {
        if (Input.GetButtonDown("Jump"))
        {
            if (playerPhysics.grounded)
                iJumpCounter = 0;

            iJumpCounter++;
            if (iJumpCounter <= 2)
            {
                bJumping = true;
                currentSpeed.y = jumpHeight / jumpTime;
            }
        }
    }

    void handleMovement(float x)
    {
        float absX = Mathf.Abs(x);
        float velocity = PlayerVelocity;

        targetSpeed.x = velocity * absX;
        currentSpeed.x = IncrementTowards(currentSpeed.x, targetSpeed.x, acceleration.x);

        if (x < 0 && transform.eulerAngles.y < 0.1f)
            transform.RotateAround(transform.position, Vector3.up, 180);
        else if (x > 0 && transform.eulerAngles.y >= 180.0f)
            transform.RotateAround(transform.position, Vector3.up, -180);

        movementDirection.x = currentSpeed.x * Time.deltaTime;
        movementDirection.y = 0.0f;

    } //end - handleMovement

	// Update is called once per frame
	void Update () {

        float x = Input.GetAxis("Horizontal");
        float deltaTime = Time.deltaTime;

        handleJump();

        movementDirection.y = 0.0f;
        //jumps & gravitation
        //we start with jumpVelocity, now we decrease it
        if (currentSpeed.y > 0.0f)
            currentSpeed.y -= acceleration.y * Time.deltaTime;
        else
        {
            if (playerPhysics.grounded == false && playerPhysics.onSlope == false)
            {
                //we are falling if we have negative speedY and we are not grounded
                if (bCanJump)
                {
                    float distanceGround = playerPhysics.GetDistanceToGround();
                    if (distanceGround > 0.5f)
                        bFalling = true;

                }
                //we want to gain speed if we are falling
                currentSpeed.y -= acceleration.y * Time.deltaTime;
            }
            else
            {
                //movement is handled earlier, if we jump we need to know if we go to idle
                bFalling = false;
                //reset to normal gravity 
                currentSpeed.y = -acceleration.y * Time.deltaTime;
            }
            bJumping = false;
        }

        handleMovement(x);

        movementDirection.y = currentSpeed.y * Time.deltaTime;
        lastDirection = playerPhysics.Move(movementDirection);
    }
}
