using UnityEngine;
using System.Collections;

public class HoverPlatform : MonoBehaviour {

    public float movementSpeed = 1.0f;
    public bool bIsActive = true;
    public enum PlatformType
    {
        Vertical = 0,
        Horizontal = 1
    }
    public PlatformType platformType;

    private float currentRadiant = 0.0f;
    private int direction = 1;

    private Vector2 Size;
	// Use this for initialization
	void Start () {
        currentRadiant = 0.0f;
        Size = transform.parent.FindChild("Stange").gameObject.GetComponent<BoxCollider2D>().size;
        
    }
	
	// Update is called once per frame
	void Update ()
    {
        if (!bIsActive)
            return;

        if (currentRadiant >= Mathf.PI)
            direction = -1;
        else if (currentRadiant <= 0)
            direction = 1;

        currentRadiant += Time.deltaTime * direction * movementSpeed;
        float transition = Mathf.Sin(currentRadiant);

        float posY = 0.0f;
        float posX = 0.0f;

        if (platformType == PlatformType.Vertical)
        {
            posY = Mathf.Lerp(-Size.y/2, Size.y/2, transition);
        }
        else if (platformType == PlatformType.Horizontal)
        {
            posX = Mathf.Lerp(-Size.x, Size.x, transition);
        }

        transform.localPosition = new Vector3(posX,posY,0.0f);
    }

}

