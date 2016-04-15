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
    private Vector3 Scale;
    private Vector3 Pos;

	// Use this for initialization
	void Start () {
        currentRadiant = 0.0f;
        Size = transform.parent.FindChild("Stange").gameObject.GetComponent<BoxCollider2D>().size;
        Scale = transform.parent.FindChild("Stange").gameObject.transform.localScale;
        Pos = transform.parent.FindChild("Stange").gameObject.transform.localPosition;
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
            float sizeY = Pos.y + Size.y * Scale.y;
            posY = Mathf.Lerp(-sizeY/2, sizeY/2, transition);
        }
        else if (platformType == PlatformType.Horizontal)
        {
            float sizeX = Pos.x + Size.x * Scale.x;
            posX = Mathf.Lerp(-sizeX/2, sizeX/2, transition);
        }

        transform.localPosition = new Vector3(posX,posY,0.0f);
    }

}

