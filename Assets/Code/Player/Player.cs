using UnityEngine;
using System.Collections;

[RequireComponent(typeof(PlayerPhysics))]
public class Player : MonoBehaviour {

    public float PlayerVelocity = 1.0f;

    //[HideInInspector]
    public PlayerPhysics playerPhysics;
    private int iJumpCounter = 0;

    public float jumpHeight = 8.0f;
    public float jumpTime = 2.0f;

    protected Vector2 targetSpeed = Vector2.zero;
    protected Vector2 currentSpeed = Vector2.zero;
    public Vector2 acceleration = Vector2.zero;
    private Vector2 movementDirection;

    public int maxEnergy = 10;
    private int currentEnergy = 0;

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

    void handleEnergySteal()
    {
        if (Input.GetButtonDown("StealEnergy"))
            transform.GetChild(2).gameObject.GetComponent<BoxCollider2D>().enabled = true;
        else if (Input.GetButtonUp("StealEnergy"))
            transform.GetChild(2).gameObject.GetComponent<BoxCollider2D>().enabled = false;
    }

    void handleEnergyProvide()
    {
        if (Input.GetButtonDown("ProvideEnergy"))
            transform.GetChild(2).gameObject.GetComponent<BoxCollider2D>().enabled = true;
        else if (Input.GetButtonUp("ProvideEnergy"))
            transform.GetChild(2).gameObject.GetComponent<BoxCollider2D>().enabled = false;
    }

    void handleJump()
    {
        if (Input.GetButtonDown("Jump"))
        {
            playerPhysics.bAttached = false;
            transform.parent = null;

            if (playerPhysics.grounded)
                iJumpCounter = 0;

            iJumpCounter++;
            if (iJumpCounter <= 2)
            {
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

    void FixedUpdate()
    {

    }

	// Update is called once per frame
	void Update () {

        float x = Input.GetAxis("Horizontal");
        float deltaTime = Time.deltaTime;

        handleJump();

        if (playerPhysics.grounded)
        {
            handleEnergySteal();
            handleEnergyProvide(); 
        }

        movementDirection.y = 0.0f;
        //jumps & gravitation
        //we start with jumpVelocity, now we decrease it
        if (currentSpeed.y > 0.0f)
            currentSpeed.y -= acceleration.y * deltaTime;
        else
        {
            if (playerPhysics.grounded == false && playerPhysics.onSlope == false)
            {
                //we are falling if we have negative speedY and we are not grounded
                /*if (bCanJump)
                {
                    float distanceGround = playerPhysics.GetDistanceToGround();
                    if (distanceGround > 0.5f)
                        bFalling = true;

                }*/
                //we want to gain speed if we are falling
                currentSpeed.y -= acceleration.y * deltaTime;
            }
            else
            {
                //movement is handled earlier, if we jump we need to know if we go to idle
                //bFalling = false;
                //reset to normal gravity 
                currentSpeed.y = -acceleration.y * deltaTime;
            }
            //bJumping = false;
        }

        handleMovement(x);

        movementDirection.y = currentSpeed.y * deltaTime;
        playerPhysics.Move(movementDirection);


        Camera.main.transform.position = new Vector3(transform.position.x, transform.position.y, Camera.main.transform.position.z);
    }

    void msg_energySteal(int energy)
    {
        currentEnergy += energy;
        currentEnergy = Mathf.Clamp(currentEnergy, 0, maxEnergy);
    }

    void msg_energyConsume(int energy)
    {
        currentEnergy -= energy;
        if (currentEnergy < 0)
            currentEnergy = 0;
    }
    void msg_attach()
    {

    }

    void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.tag == "Hover")
        {
            playerPhysics.bAttached = true;
            transform.parent = collision.gameObject.transform;
        }
    }

}
