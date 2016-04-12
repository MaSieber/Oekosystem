using UnityEngine;
using System.Collections;

public class Parkur : MonoBehaviour {

    //Inspector Settings
    public float fadeOutModifier = 1.0f;
    enum Events
    {
        None = 0,
        EnergySuckOut = 1
    }
    private Events eventType;

	// Use this for initialization
	void Start () {
        eventType = Events.None;
    }
	
	// Update is called once per frame
	void Update ()
    {
        float deltaTime = Time.deltaTime;

        switch (eventType)
        {
            case Events.EnergySuckOut:
                Color col = this.gameObject.GetComponent<SpriteRenderer>().material.GetColor("Tint");
                col.a -= deltaTime * fadeOutModifier;

                if (col.a < 0.0f)
                { 
                    col.a = 0.0f;
                    eventType = Events.None;
                }

                break;
        }
	}

    void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.tag == "Player")
        {
            eventType = Events.EnergySuckOut;
        }
    }


}
